#pragma once

#include <GL\glew.h>
#include <glfw3.h>

#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"

class Shape
{
protected:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLsizei indices;

public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::quat rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	virtual void Draw();

	~Shape();
};

class Plane : public Shape
{
public:
	Plane();
};

class Cube : public Shape
{
public:
	Cube();
};

class Camera
{
public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, -1.0f };

	glm::vec3 GetForwardVector();
	glm::vec3 GetRightVector();
	glm::vec3 GetUpVector();
};

class Light
{
public:
	glm::vec3* position;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	GLfloat intensity = 1.0f;
};