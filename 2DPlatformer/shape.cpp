#define _USE_MATH_DEFINES
#define NUM_SEGMENTS 100
//#define _DEBUG_NUM_SEGMENTS

#include <glfw3.h>
#include "shape.h"

#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

//#include "glm/gtx/string_cast.hpp"	// Debug
//#include <iostream>					//

namespace shape
{
#pragma region Shape

	Shape::Shape(glm::vec3 position, std::vector<glm::vec4> vertices, shape::color color)
		: position(position), vertices(vertices), collider(), color(color) 
	{
		glm::vec3 lowestV = { 1.0f, 1.0f, 0.0f };
		glm::vec3 highestV = { -1.0f, -1.0f, 0.0f };

		for (glm::vec3 vertex : vertices)
		{
			if (vertex.x < lowestV.x)
				lowestV.x = vertex.x;

			if (vertex.y < lowestV.y)
				lowestV.y = vertex.y;

			if (vertex.x > highestV.x)
				highestV.x = vertex.x;

			if (vertex.y > highestV.y)
				highestV.y = vertex.y;
		}

		collider.push_back(glm::vec4(lowestV, 1));
		collider.push_back(glm::vec4(highestV, 1));
	}

	glm::vec3 Shape::getPosition()
	{
		return this->position;
	}

	void Shape::setPosition(glm::vec3 position)
	{
		this->position = position;
	}

	std::vector<glm::vec4> Shape::getVertices()
	{
		return this->vertices;
	}

	void Shape::setVertices(std::vector<glm::vec4> vertices)
	{
		this->vertices = vertices;
	}

	std::vector<glm::vec4> Shape::getCollider()
	{
		return this->collider;
	}

	shape::color Shape::getColor()
	{
		return this->color;
	}

	void Shape::setColor(shape::color color)
	{
		this->color = color;
	}

	bool Shape::isDrawn()
	{
		return this->drawn;
	}

	bool Shape::isVisible()
	{
		return this->visible;
	}

	void Shape::isVisible(bool value)
	{
		this->visible = value;
	}

	bool Shape::canCollide()
	{
		return this->collision;
	}

	void Shape::canCollide(bool value)
	{
		this->collision = value;
	}

	bool Shape::isDeadly()
	{
		return this->deadly;
	}

	void Shape::isDeadly(bool value)
	{
		this->deadly = value;
	}

	void Shape::translate(glm::vec3 translVec) 
	{
		glm::mat4 translMat = glm::translate(glm::mat4(1.0), translVec);
		position = translMat * glm::vec4(position, 1);
	}

	bool Shape::collide(Shape* shape)
	{
		return !(shape->getCollider()[1].y + shape->getPosition().y < this->collider[0].y + this->position.y
			  || shape->getCollider()[0].x + shape->getPosition().x > this->collider[1].x + this->position.x
			  || shape->getCollider()[0].y + shape->getPosition().y > this->collider[1].y + this->position.y
			  || shape->getCollider()[1].x + shape->getPosition().x < this->collider[0].x + this->position.x);
	}

	bool collide(std::vector<glm::vec4> vertices, Shape* shape)
	{
		return !(shape->getCollider()[1].y + shape->getPosition().y < vertices[0].y
			  || shape->getCollider()[0].x + shape->getPosition().x > vertices[1].x
			  || shape->getCollider()[0].y + shape->getPosition().y > vertices[1].y
			  || shape->getCollider()[1].x + shape->getPosition().x < vertices[0].x);
	}

#pragma endregion

#pragma region Polygon

	Polygon::Polygon(glm::vec3 position, std::vector<glm::vec4> vertices, shape::color color)
		: Shape(position, vertices, color) {}

	void Polygon::draw()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);

		for (size_t i = 0; i < vertices.size(); i++)
		{
			glm::vec4 v = posMat * vertices[i];
			glVertex2f(v.x, v.y);
		}

		glEnd();

		drawn = true;
	}

	void Polygon::drawSlow()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);

		if (seg < vertices.size()) seg++;
		else drawn = true;

		for (size_t i = 0; i < seg; i++)
		{
			glBegin(GL_LINE_LOOP);

			if (i + 1 < vertices.size())
			{
				glm::vec4 vi0 = posMat * vertices[i];
				glm::vec4 vi1 = posMat * vertices[i + 1];

				glVertex2f(vi0.x, vi0.y);
				glVertex2f(vi1.x, vi1.y);
			}

			else
			{
				glm::vec4 v0 = posMat * vertices[0];
				glm::vec4 vi0 = posMat * vertices[i];

				glVertex2f(vi0.x, vi0.y);
				glVertex2f(v0.x, v0.y);
			}

			glEnd();
		}
	}

	void Polygon::fill()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);
		glBegin(GL_POLYGON);

		for (size_t i = 0; i < vertices.size(); i++)
		{
			glm::vec4 v = posMat * vertices[i];
			glVertex2f(v.x, v.y);
		}

		glEnd();

		drawn = true;
	}

#pragma endregion

#pragma region Triangle

	Triangle::Triangle(glm::vec3 position, std::vector<glm::vec4> vertices, shape::color color)
		: Shape(position, vertices, color) {}

	void Triangle::draw()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);

		for (size_t i = 0; i < vertices.size(); i++)
		{
			glm::vec4 v = posMat * vertices[i];
			glVertex2f(v.x, v.y);
		}

		glEnd();

		drawn = true;
	}

	void Triangle::drawSlow()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);

		if (seg < vertices.size()) seg++;
		else drawn = true;

		for (int i = 0; i < seg; i++)
		{
			glBegin(GL_LINE_LOOP);

			glm::vec4 v0 = posMat * vertices[0];
			glm::vec4 vi0 = posMat * vertices[i];
			glm::vec4 vi1 = posMat * vertices[i + 1];

			if (i + 1 < vertices.size())
			{
				glVertex2f(vi0.x, vi0.y);
				glVertex2f(vi1.x, vi1.y);
			}

			else
			{
				glVertex2f(vi0.x, vi0.y);
				glVertex2f(v0.x, v0.y);
			}

			glEnd();
		}
	}

	void Triangle::fill()
	{
		glm::mat4 posMat = glm::translate(glm::mat4(1.0), position);

		glColor3f(color.r, color.g, color.b);
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < vertices.size(); i++)
		{
			glm::vec4 v = posMat * vertices[i];
			glVertex2f(v.x, v.y);
		}

		glEnd();

		drawn = true;
	}

#pragma endregion

#pragma region Circle

	Circle::Circle(glm::vec3 position, float radius, shape::color color)
		: Shape(position, std::vector<glm::vec4>(), color), r(radius) 
	{
		glm::vec3 lowestV = { -r, -r, 0.0f };
		glm::vec3 highestV = { r, r, 0.0f };

		collider.push_back(glm::vec4(lowestV, 1));
		collider.push_back(glm::vec4(highestV, 1));
	}

	float Circle::getRadius()
	{
		return Circle::r;
	}

	void Circle::draw()
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);

		for (int i = 0; i < NUM_SEGMENTS; i++)
		{
			// theta in radians
			float theta = 2.0f * M_PI * (float(i) / float(NUM_SEGMENTS));

			float x = r * cosf(theta);
			float y = r * sinf(theta);

			glVertex2f(position.x + x, position.y + y);
		}

		glEnd();

		drawn = true;

#ifdef _DEBUG_NUM_SEGMENTS

		for (int i = 0; i < NUM_SEGMENTS; i++)
		{
			glBegin(GL_LINE_LOOP);
			float theta = 2.0f * M_PI * (float(i) / float(NUM_SEGMENTS));

			float x = r * cosf(theta);
			float y = r * sinf(theta);

			glVertex2f(position.x, position.y);
			glVertex2f(position.x + x, position.y + y);

			glEnd();
		}

		for (int i = 0; i < NUM_SEGMENTS; i++)
		{
			glBegin(GL_LINE_LOOP);
			float theta = 2.0f * M_PI * (float(i) / float(NUM_SEGMENTS));

			float x = r * cosf(theta);
			float y = r * sinf(theta);

			glVertex2f(position.x + x, position.y);
			glVertex2f(position.x + x, position.y + y);

			glEnd();
		}

#endif
	}

	void Circle::drawSlow()
	{
		glColor3f(color.r, color.g, color.b);

		static int seg = 0;
		if (seg < NUM_SEGMENTS) seg++;
		else drawn = true;

		for (int i = 0; i < seg; i++)
		{
			glBegin(GL_LINE_LOOP);
			// theta in radians
			float theta1 = 2.0f * M_PI * (float(i) / float(NUM_SEGMENTS));
			float theta2 = 2.0f * M_PI * (float(i+1) / float(NUM_SEGMENTS));

			float x1 = r * cosf(theta1);
			float y1 = r * sinf(theta1);

			float x2 = r * cosf(theta2);
			float y2 = r * sinf(theta2);

			glVertex2f(position.x + x1, position.y + y1);
			glVertex2f(position.x + x2, position.y + y2);
			glEnd();
		}
	}

	void Circle::fill()
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_POLYGON);

		for (int i = 0; i < NUM_SEGMENTS; i++)
		{
			// theta in radians
			float theta = 2.0f * M_PI * (float(i) / float(NUM_SEGMENTS));

			float x = r * cosf(theta);
			float y = r * sinf(theta);

			glVertex2f(position.x + x, position.y + y);
		}

		glEnd();

		drawn = true;
	}

#pragma endregion

}