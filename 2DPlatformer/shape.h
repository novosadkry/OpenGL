#pragma once
#include "color.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <vector>

namespace shape
{
	class Shape
	{
	protected:
		glm::vec3				position;
		std::vector<glm::vec4>	vertices; // Relative to position
		std::vector<glm::vec4>	collider; // Relative to position
		shape::color			color;

		unsigned int seg = 0;
		bool drawn = false;
		bool visible = true;
		bool collision = true;
		bool deadly = false;

	public:
		Shape(glm::vec3, std::vector<glm::vec4>, shape::color);

		glm::vec3 getPosition();
		void setPosition(glm::vec3);

		std::vector<glm::vec4> getVertices();
		void setVertices(std::vector<glm::vec4>);

		std::vector<glm::vec4> getCollider();

		shape::color getColor();
		void setColor(shape::color);

		bool isDrawn();

		bool isVisible();
		void isVisible(bool);

		bool canCollide();
		void canCollide(bool);

		bool isDeadly();
		void isDeadly(bool);

		virtual void draw() = 0;
		virtual void drawSlow() = 0;
		virtual void fill() = 0;

		bool collide(shape::Shape*);
		
		void translate(glm::vec3);
	};

	bool collide(std::vector<glm::vec4>, shape::Shape*);

	class Polygon : public Shape
	{
	public:
		Polygon(glm::vec3, std::vector<glm::vec4>, shape::color);

		void draw();
		void drawSlow();
		void fill();
	};

	class Triangle : public Shape
	{
	public:
		Triangle(glm::vec3, std::vector<glm::vec4>, shape::color);

		void draw();
		void drawSlow();
		void fill();
	};

	class Circle : public Shape
	{
	private:
		float r;
	public:
		Circle(glm::vec3, float r, shape::color);

		float getRadius();

		void drawSlow();
		void draw();
		void fill();
	};
}