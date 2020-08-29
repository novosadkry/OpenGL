#pragma once

#include <glm\glm.hpp>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

namespace materials
{
    extern Material none;
    extern Material copper;
    extern Material white_plastic;
    extern Material brass;
    extern Material pearl;
}