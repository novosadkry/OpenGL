#pragma once

#include "shape.h"

#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Object
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::vector<GLuint> vert_indices;
	std::vector<GLuint> normal_indices;
	std::vector<GLuint> uv_indices;
};

void LoadObjectFromFile(Object* obj, const char* path);

std::vector<Vertex> GenerateObjectVertices(Object& obj);