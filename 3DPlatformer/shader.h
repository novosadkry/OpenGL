#pragma once

#include <GL\glew.h>

#include <string>
#include <vector>

namespace shaders
{
	extern GLuint base_shader;
}

struct ShaderSource
{
	uint16_t type;
	std::string source;
	
	friend ShaderSource& operator<<(ShaderSource& ss, const std::string& other)
	{
		ss.source.append(other);
		return ss;
	}
};

std::vector<ShaderSource> ParseShader(const char* path);

GLuint CompileShader(GLenum type, const char* source);

GLuint CreateShaderProgram(const char* path);