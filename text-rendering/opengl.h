#ifndef OPENGL_H_CUSTOM
#define OPENGL_H_CUSTOM 1

#include "khrplatform.h"
#include "glad.h"
#include "glfw3.h"
#include "glfw3native.h"
#include <iostream>

#ifdef _WIN32
typedef unsigned int uint;
#endif

void gl_print_errors()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cerr << "OpenGL error code: " << err << std::endl;
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

	init_result init(const char *title = "", unsigned int s_width = 720, unsigned int s_height = 480,
					 GLFWframebuffersizefun on_win_resize = framebuffer_size_callback)
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
		glfwSetFramebufferSizeCallback(window, on_win_resize);
		return init_success;
	}

	uint get_width()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return width;
	}

	uint get_height()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return height;
	}

	static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
};

struct shader_vf
{
	uint shader_id = GL_NONE;
	bool init(const char *v_src, const char *f_src)
	{
		shader_id = glCreateProgram();
		uint vs = compile_shader(v_src, GL_VERTEX_SHADER);
		if (vs == GL_NONE)
			return false;
		uint fs = compile_shader(f_src, GL_FRAGMENT_SHADER);
		if (fs == GL_NONE)
			return false;
		glAttachShader(shader_id, vs);
		glAttachShader(shader_id, fs);
		glLinkProgram(shader_id);
		glValidateProgram(shader_id);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return true;
	}
	void use()
	{
		glUseProgram(shader_id);
	}
	void active_texture(uint gl_tex_unit)
	{
		glActiveTexture(gl_tex_unit);
	}
	void bind_texture(uint tex_id)
	{
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}
	uint gsl(const char *name)
	{
		return glGetUniformLocation(shader_id, name);
	}

	void float1(const char *n, float v)
	{
		glUniform1f(gsl(n), v);
	}
	void float2(const char *n, float a, float b)
	{
		glUniform2f(gsl(n), a, b);
	}
	void float3(const char *n, float a, float b, float c)
	{
		glUniform3f(gsl(n), a, b, c);
	}
	void float4(const char *n, float a, float b, float c, float d)
	{
		glUniform4f(gsl(n), a, b, c, d);
	}

	void int1(const char *n, int v)
	{
		glUniform1i(gsl(n), v);
	}
	void int2(const char *n, int a, int b)
	{
		glUniform2i(gsl(n), a, b);
	}
	void int3(const char *n, int a, int b, int c)
	{
		glUniform3i(gsl(n), a, b, c);
	}
	void int4(const char *n, int a, int b, int c, int d)
	{
		glUniform4i(gsl(n), a, b, c, d);
	}

	void bool1(const char *n, bool v)
	{
		glUniform1i(gsl(n), v);
	}
	void bool2(const char *n, bool a, bool b)
	{
		glUniform2i(gsl(n), a, b);
	}
	void bool3(const char *n, bool a, bool b, bool c)
	{
		glUniform3i(gsl(n), a, b, c);
	}
	void bool4(const char *n, bool a, bool b, bool c, bool d)
	{
		glUniform4i(gsl(n), a, b, c, d);
	}

	void mat4f(const char *name, float mat[4][4])
	{
		glUniformMatrix4fv(gsl(name), 1, false, &mat[0][0]);
	}

	static uint compile_shader(const char *src, uint shader_type)
	{
		uint id = glCreateShader(shader_type);
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int el;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &el);
			char *message = (char *)alloca(el * sizeof(char));
			glGetShaderInfoLog(id, el, &el, message);
			std::cerr << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown")) << " shader: " << message << std::endl;
			glDeleteShader(id);
			return GL_NONE;
		}
		return id;
	}
};

#endif