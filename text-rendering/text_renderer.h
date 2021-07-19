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

const char *TEST_VERT_SHADER =
	"#version 330 core\nlayout (location = 0) in vec2 vert;void main(){gl_Position = vec4(vert.x, vert.y, 0.0, 1.0);}";
const char *TEST_FRAG_SHADER =
	"#version 330 core\nlayout (location = 0) out vec4 colour;void main(){colour = vec4(0.4, 0.3, 0.8, 1.0);}";

using namespace galg;
struct text_renderer
{
	struct char_info
	{
		char ch = ' ';
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
	};

	gl_window window;
	char_info **screen = nullptr;
	size_t width = 0;
	size_t height = 0;
	uint vbo = GL_NONE;
	uint vao = GL_NONE;
	char_set glyphs;
	shader_vf shader;

	text_renderer() {}
	~text_renderer()
	{
		for (size_t i = 0; i < height; ++i)
			delete[] screen[i];
		delete[] screen;
	}

	bool init(const char *title, size_t char_width, size_t char_height, const char *ttf_font_path, size_t text_res = 64)
	{
		switch (window.init(title, 500, 500))
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

		width = char_width;
		height = char_height;

		screen = new char_info *[height];
		for (size_t i = 0; i < height; ++i)
			screen[i] = new char_info[width];

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
			glyphs.characters.insert(std::pair<char, char_set::character>(c, ch));
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

	// void draw_char(char draw, fp_num x, fp_num y, fp_num scale, vec3 col)
	// {
	// 	shader.use();
	// 	mat4 ortho = mat4::orthographic(0, 500, 0, 500);
	// 	shader.mat4f("projection", ortho.m);
	// 	char_set::character &ch = glyphs.get(draw);
	// 	float xpos = x + ch.bearing.x() * scale;
	// 	float ypos = y - (ch.size.y() - ch.bearing.y()) * scale;
	// 	float w = ch.size.x() * scale;
	// 	float h = ch.size.y() * scale;

	// 	float vertices[6][4] = {
	// 		{xpos, ypos + h, 0.0f, 0.0f},
	// 		{xpos, ypos, 0.0f, 1.0f},
	// 		{xpos + w, ypos, 1.0f, 1.0f},

	// 		{xpos, ypos + h, 0.0f, 0.0f},
	// 		{xpos + w, ypos, 1.0f, 1.0f},
	// 		{xpos + w, ypos + h, 1.0f, 0.0f}};

	// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 	glBindVertexArray(vao);
	// 	glDrawArrays(GL_TRIANGLES, 0, 6);
	// }

	void render_text(const char *text, fp_num x, const fp_num y, const fp_num scale, const vec3 &col)
	{
		shader.use();
		shader.active_texture(GL_TEXTURE0);
		mat4 ortho = mat4::orthographic(0, 720, 0, 480, 1, -1);
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
};

#endif