#include "shader.h"
#include "error.h"

#include <iostream>
#include <fstream>

GLuint shaders::base_shader;

ShaderSource* ParseShader(const char* path)
{
    ShaderSource* shaders = new ShaderSource[2];
    ShaderType current_type = ShaderType::NONE;

    std::ifstream file_stream(path);
    std::string line;

    while (std::getline(file_stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                current_type = ShaderType::VERTEX;

            if (line.find("fragment") != std::string::npos)
                current_type = ShaderType::FRAGMENT;
        }

        else
        {
            ASSERT(current_type != ShaderType::NONE);
            shaders[(int)current_type].append(line + "\n");
        }
    }

    return shaders;
}

GLuint CompileShader(GLenum type, const ShaderSource& source)
{
    GLuint id = glCreateShader(type);
    const char* _source = source.c_str();

    glShaderSource(id, 1, &_source, nullptr);
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

GLuint CreateShaderProgram()
{
    ShaderSource* shaders = ParseShader("res/shaders/base.shader");

    GLuint program = glCreateProgram();

    GLuint vs = CompileShader(GL_VERTEX_SHADER, shaders[(int)ShaderType::VERTEX]);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, shaders[(int)ShaderType::FRAGMENT]);

    ASSERT(vs != 0);
    ASSERT(fs != 0);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    delete[] shaders;

    return program;
}