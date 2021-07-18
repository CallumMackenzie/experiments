#ifndef TEXT_RENDERER_H_
#define TEXT_RENDERER_H_ c1
#include "opengl.h"
#include <iostream>

struct text_renderer
{
	struct char_info
	{
		char text = ' ';
	};

	gl_window window;
	char_info **screen;
	size_t width = 0;
	size_t height = 0;

	text_renderer() {}
	~text_renderer()
	{
		for (size_t i = 0; i < height; ++i)
			delete[] screen[i];
		delete[] screen;
	}

	bool init(const char *title, size_t char_width, size_t char_height)
	{
		switch (window.init(title))
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

		screen = new char_info *[char_height];
		for (size_t i = 0; i < char_height; ++i)
			screen[i] = new char_info[char_width];

		width = char_width;
		height = char_height;

		return true;
	}

	void render_text()
	{
		window.clear();

		window.swap_buffers();
	}
};

#endif