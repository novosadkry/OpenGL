#pragma once

#include "shape.h"

#include <vector>

class Object
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	std::vector<GLuint> vert_indices;
	std::vector<GLuint> normal_indices;
};

void LoadObjectFromFile(Object* obj, const char* path);

std::vector<glm::vec3> GenerateObjectVertices(Object& obj);