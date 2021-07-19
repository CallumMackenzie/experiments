#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_ c1
#include "opengl.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "galg.h"
#include <unordered_map>
#include <iostream>

const char *GLYPH_VERT_SHADER =
	"#version 330 core\nlayout (location = 0) in vec4 vert;out vec2 uv;uniform mat4 projection;void main(){gl_Position = projection * vec4(vert.x, vert.y, 0.0, 1.0);uv = vert.zw;}";
const char *GLYPH_FRAG_SHADER =
	"#version 330 core\nin vec2 uv;layout (location = 0) out vec4 colour;uniform sampler2D ch;uniform vec3 text_colour;void main(){vec4 sm = vec4(1.0,1.0,1.0,texture(ch, uv).r);colour = vec4(text_colour,1.0) * sm;}";

using namespace galg;
struct text_renderer
{
	struct char_info
	{
		inline static char ctr = 32;
		char ch = ' ';
		vec3 col {0.4, 0.25, 0.13};
		char_info(){
			if (ctr >= 127)
				ctr = 32;
			ch = ++ctr;
		}
	};
	struct char_set
	{
		struct character
		{
			uint tex_id = GL_NONE;
			vec2 size;
			vec2 bearing;
			uint advance;
		};
		std::unordered_map<char, character> characters;
		character &get(char c)
		{
			return characters[c];
		}
		void set(char c, character v)
		{
			characters.insert(std::pair<char, character>(c, v));
		}
	};

	gl_window window;
	char_info **screen = nullptr;
	uint vbo = GL_NONE;
	uint vao = GL_NONE;
	char_set glyphs;
	shader_vf shader;
	const vec2 char_size;
	vec2 margin{30, 30};
	vec2 pixel_size;

	text_renderer(size_t char_width, size_t char_height) : char_size(char_width, char_height)
	{
		screen = new char_info *[(size_t)char_size.y()];
		for (size_t i = 0; i < char_size.y(); ++i)
			screen[i] = new char_info[char_width];
	}
	~text_renderer()
	{
		for (size_t i = 0; i < char_size.y(); ++i)
			delete[] screen[i];
		delete[] screen;
	}

	bool init(const char *title, const char *ttf_font_path, size_t text_res = 64, size_t pixel_width = 720, size_t pixel_height = 480)
	{
		switch (window.init(title, pixel_size.x() = pixel_width, pixel_size.y() = pixel_height))
		{
		case gl_window::init_success:
			break;
		case gl_window::null_window_ptr:
			std::cerr << "Window pointer was null after setup." << std::endl;
			return false;
		case gl_window::no_gl_funcs_found:
			std::cerr << "OpenGL may not be supported on this system." << std::endl;
			return false;
		default:
			return false;
		}

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cerr << "Could not init FreeType library." << std::endl;
			return false;
		}
		FT_Face face;
		if (FT_New_Face(ft, ttf_font_path, 0, &face))
		{
			std::cerr << "Failed to load font." << std::endl;
			FT_Done_FreeType(ft);
			return false;
		}
		FT_Set_Pixel_Sizes(face, 0, text_res);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; ++c)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cerr << "Failed to load Glyph: " << c << std::endl;
				continue;
			}
			uint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			char_set::character ch{tex,
								   vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
								   vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
								   (uint)face->glyph->advance.x};
			glyphs.set(c, ch);
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		if (!shader.init(GLYPH_VERT_SHADER, GLYPH_FRAG_SHADER))
		{
			std::cerr << "Error compiling glyph shader." << std::endl;
			return false;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}

	void render_text(const char *text, fp_num x, const fp_num y, const fp_num scale, const vec3 &col)
	{
		shader.use();
		shader.active_texture(GL_TEXTURE0);
		mat4 ortho = mat4::orthographic(0, pixel_size.x(), 0, pixel_size.y(), 1, -1);
		shader.mat4f("projection", ortho.m);
		shader.int1("ch", 0);
		shader.float3("text_colour", col.x(), col.y(), col.z());

		uint char_ctr = 0;
		char draw = text[0];
		while (draw != '\0')
		{
			char_set::character &ch = glyphs.get(draw);
			float xpos = x + ch.bearing.x() * scale;
			float ypos = y - (ch.size.y() - ch.bearing.y()) * scale;
			float w = ch.size.x() * scale;
			float h = ch.size.y() * scale;

			float vertices[6][4] = {
				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos, ypos, 0.0f, 1.0f},
				{xpos + w, ypos, 1.0f, 1.0f},

				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos + w, ypos, 1.0f, 1.0f},
				{xpos + w, ypos + h, 1.0f, 0.0f}};

			shader.bind_texture(ch.tex_id);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * scale;
			draw = text[++char_ctr];
		}
	}

	void set_char(size_t x, size_t y, char c)
	{
		screen[y][x].ch = c;
	}

	void render_screen()
	{
		pixel_size.x() = window.get_width();
		pixel_size.y() = window.get_height();
		vec2 px_size = pixel_size - (margin * 2);
		for (size_t y = 0; y < char_size.y(); ++y)
			for (size_t x = 0; x < char_size.x(); ++x)
			{
				const char txt[2]{screen[y][x].ch, '\0'};
				vec2 new_pos = (vec2(x, y) * (px_size / char_size));
				render_text(txt, new_pos.x() + margin.x(), (px_size.y() - new_pos.y()) + margin.y(), 0.3, screen[y][x].col);
			}
	}
};

#endif