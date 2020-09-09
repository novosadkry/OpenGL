#include "shader.h"
#include "error.h"

#include <iostream>
#include <fstream>

namespace shaders
{
    GLuint base_shader;
}

std::unique_ptr<std::vector<ShaderSource>> ParseShader(const char* path)
{
    auto shaders = std::make_unique<std::vector<ShaderSource>>();

    std::ifstream file_stream(path);
    std::string line;

    ShaderSource current_shader = ShaderSource();

    while (std::getline(file_stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (current_shader.type != 0)
                shaders->push_back(current_shader);

            current_shader.source.clear();

            if (line.find("vertex") != std::string::npos)
                current_shader.type = GL_VERTEX_SHADER;

            else if (line.find("fragment") != std::string::npos)
                current_shader.type = GL_FRAGMENT_SHADER;

            else if (line.find("geometry") != std::string::npos)
                current_shader.type = GL_GEOMETRY_SHADER;
        }

        else
        {
            ASSERT(current_shader.type != 0);
            current_shader.source.append(line + "\n");
        }
    }
    
    if (current_shader.type != 0)
        shaders->push_back(current_shader);

    return shaders;
}

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        GLsizei length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        GLchar* msg = (GLchar*)alloca(sizeof(GLchar) * length);
        glGetShaderInfoLog(id, length, nullptr, msg);

        std::cout << msg << std::endl;
        
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint CreateShaderProgram(const char* path)
{
    auto shaders = ParseShader(path);

    GLuint program = glCreateProgram();

    for (ShaderSource shaderSource : *shaders)
    {
        GLuint shader = CompileShader(shaderSource.type, shaderSource.source.c_str());

        ASSERT(shader != 0);
        glAttachShader(program, shader);

        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(shader);
    }

    return program;
}