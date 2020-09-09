#pragma once

#include <GL\glew.h>
#include <glfw3.h>

#include <string>
#include <vector>
#include <memory>

namespace shaders
{
	extern GLuint base_shader;
}

struct ShaderSource
{
	unsigned int type;
	std::string source;
};

std::unique_ptr<std::vector<ShaderSource>> ParseShader(const char* path);

GLuint CompileShader(GLenum type, const char* source);

GLuint CreateShaderProgram(const char* path);