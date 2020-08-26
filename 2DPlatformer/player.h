#pragma once

#include "shape.h"

namespace player
{
	class Player
	{
	private:
		shape::Shape* shape;
		glm::vec3 velocity;
		float mass;

		bool dead;
		bool ground;

	public:
		Player(shape::Shape*);

		shape::Shape* getShape();

		glm::vec3 getVelocity();
		void setVelocity(glm::vec3);

		float getMass();
		void setMass(float);

		bool isDead();
		void isDead(bool);

		bool isGrounded();
		void isGrounded(bool);

		void move();
		void applyForce(glm::vec3);
	};
}