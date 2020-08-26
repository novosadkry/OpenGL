#pragma once

#include <GL\glew.h>
#include <glfw3.h>

#include <string>

namespace shaders
{
	extern GLuint base_shader;
}

typedef std::string ShaderSource;

enum class ShaderType
{
	NONE = -1,
	VERTEX,
	FRAGMENT
};

ShaderSource* ParseShader(const char* path);

GLuint CompileShader(GLenum type, const ShaderSource& source);

GLuint CreateShaderProgram();