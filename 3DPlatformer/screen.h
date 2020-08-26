#pragma once

#include <GL\glew.h>
#include <glfw3.h>

enum class WindowMode
{
	WINDOWED,
	FULLSCREEN,
	BORDERLESS
};

namespace screen
{
	extern int width;
	extern int height;

	extern GLFWwindow* window;

	int init(WindowMode window_mode, int width, int height, const char* title);
}