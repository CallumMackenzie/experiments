#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_ 1
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
		char ch = ' ';
		vec3fp col{(fp_num)0.6, (fp_num)0.6, (fp_num)0.6};
	};
	struct char_set
	{
		struct character
		{
			uint tex_id = GL_NONE;
			vec2ui size;
			vec2ui bearing;
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
	const vec2st char_size;
	vec2fp margin{30, 30};
	vec2fp pixel_size;
	vec2st cursor;
	fp_num font_size = (fp_num)0.2;

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
		switch (window.init(title, pixel_size.x() = (fp_num)pixel_width, pixel_size.y() = (fp_num)pixel_height))
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
								   vec2ui(face->glyph->bitmap.width, face->glyph->bitmap.rows),
								   vec2ui(face->glyph->bitmap_left, face->glyph->bitmap_top),
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

	void render_text(const char *text, fp_num x, const fp_num y, const fp_num scale, const vec3fp &col)
	{
		shader.use();
		shader.active_texture(GL_TEXTURE0);
		mat4fp ortho = mat4fp::orthographic(0, pixel_size.x(), 0, pixel_size.y(), 1, -1);
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
		pixel_size.x() = (fp_num)window.get_width();
		pixel_size.y() = (fp_num)window.get_height();
		vec2fp px_size = pixel_size - (margin * 2);
		for (size_t y = 0; y < char_size.y(); ++y)
			for (size_t x = 0; x < char_size.x(); ++x)
			{
				if (screen[y][x].ch == ' ')
					continue;
				const char txt[2]{screen[y][x].ch, '\0'};
				vec2fp new_pos = (vec2<fp_num>((fp_num)x, (fp_num)y) * (px_size / vec2fp{(fp_num)char_size.x(), (fp_num)char_size.y()}));
				render_text(txt, new_pos.x() + margin.x(), (px_size.y() - new_pos.y()) + margin.y(), font_size, screen[y][x].col);
			}
	}

	void print(const char *text)
	{
		size_t ctr = 0;
		char select = text[0];
		while (select != '\0')
		{
			if (cursor.x() >= char_size.x())
				return;
			if (select == '\n')
			{
				cursor.x() = 0;
				if (++cursor.y() >= char_size.y())
					return;
			}
			else
			{
				screen[(size_t)cursor.y()][(size_t)cursor.x()].ch = select;
				++cursor.x();
			}

			select = text[++ctr];
		}
	}

	void clear(char clear = ' ')
	{
		for (size_t i = 0; i < char_size.x(); ++i)
			for (size_t j = 0; j < char_size.y(); ++j)
				screen[j][i].ch = clear;
		home_cursor();
	}

	void home_cursor()
	{
		cursor = vec2st{0, 0};
	}
};

#endif