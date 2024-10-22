#pragma once

#include <GL\glew.h>
#include <glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

#include "material.h"

class Shape
{
protected:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLsizei indices;

public:
	Material material;

	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	virtual void Draw();

	virtual ~Shape();
};

class ShapedObject : public Shape
{
public:
	ShapedObject(const char* object_path);
};

class Camera
{
public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 direction = { 0.0f, 0.0f, -1.0f };

	float speed = 1.0f;
	float maxSpeed = 100.0f;

	glm::vec3 GetForwardVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;
};

class Light
{
public:
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	GLfloat intensity = 1.0f;
};