#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

struct Material
{
    GLuint diffuse;
    GLuint specular;
    float shininess;
    glm::vec3 ambient;
    glm::vec4 color;
};