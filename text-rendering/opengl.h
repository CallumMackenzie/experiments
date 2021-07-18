#ifndef OPENGL_H_CUSTOM
#define OPENGL_H_CUSTOM 1

#include "khrplatform.h"
#include "glad.h"
#include "glfw3.h"
#include "glfw3native.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

struct gl_window
{
	enum init_result
	{
		init_success,
		null_window_ptr,
		no_gl_funcs_found
	};

	GLFWwindow *window = nullptr;

	gl_window() {}
	~gl_window()
	{
		glfwTerminate();
	}

	bool should_close()
	{
		return glfwWindowShouldClose(window);
	}

	void set_clear_colour(long colour)
	{
		int r = (colour & 0xFF0000) >> 16;
		int g = (colour & 0x00FF00) >> 8;
		int b = (colour & 0x0000FF);
		glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, 1.0);
	}

	void clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void swap_buffers()
	{
		glfwSwapBuffers(window);
	}

	void poll_events()
	{
		glfwPollEvents();
	}

	void set_position(int screen_x, int screen_y)
	{
		glfwSetWindowPos(window, screen_x, screen_y);
	}

	init_result init(const char *title = "", unsigned int s_width = 720, unsigned int s_height = 480)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
		window = glfwCreateWindow((int)s_width, (int)s_height, title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return null_window_ptr;
		}
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return no_gl_funcs_found;
		glViewport(0, 0, (int)s_width, (int)s_height);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		return init_success;
	}
};

#endif