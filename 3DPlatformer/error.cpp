#include "error.h"

#include <GL\glew.h>
#include <glfw3.h>

#include <iostream>

void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

int glLogError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL:ERROR] (code " << error << ") at " << function << " in " << file << ":" << line << std::endl;
        return 0;
    }

    return 1;
}