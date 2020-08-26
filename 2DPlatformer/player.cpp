#include "player.h"
#include "game.h"

//#include "glm/gtx/string_cast.hpp"	// Debug
//#include <iostream>					//

namespace player
{
	Player::Player(shape::Shape* shape) 
		: shape(shape), velocity(), mass(), ground() {}

	shape::Shape* Player::getShape()
	{
		return this->shape;
	}

	glm::vec3 Player::getVelocity()
	{
		return this->velocity;
	}

	void Player::setVelocity(glm::vec3 velocity)
	{
		this->velocity = velocity;
	}

	float Player::getMass()
	{
		return this->mass;
	}

	void Player::setMass(float mass)
	{
		this->mass = mass;
	}

	bool Player::isDead()
	{
		return this->dead;
	}

	void Player::isDead(bool value)
	{
		this->dead = value;
	}

	bool Player::isGrounded()
	{
		return this->ground;
	}

	void Player::isGrounded(bool value)
	{
		this->ground = value;
	}

	// Move according to the current velocity
	void Player::move()
	{
		// Add gravity and air resistance
		this->velocity += game::gravity * game::deltaTime;
		this->velocity -= this->velocity * (game::air * game::deltaTime);

		// Speed scale matrix
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), glm::vec3(0.5f, 0.5f, 0.5f));
		glm::vec4 position = glm::vec4(this->velocity * game::deltaTime, 0);

		// Approximate of a new position
		glm::mat4 translMat = glm::translate(glm::mat4(1.0), glm::vec3(scaleMat * position));
		glm::vec3 newPos = this->shape->getPosition();
		newPos = translMat * glm::vec4(newPos, 1);

		// Screen border - X
		if (newPos.x >= 1.0f - this->shape->getCollider()[1].x
			|| newPos.x <= -1.0f - this->shape->getCollider()[0].x 
			|| newPos.y >= 1.0f - this->shape->getCollider()[1].y
			|| newPos.y <= -1.0f - this->shape->getCollider()[0].y)	
			this->velocity = { 0, 0, 0 };

		// Collision
		for (shape::Shape* shape : *game::shapes)
		{
			if (shape->canCollide())
			{
				glm::mat4 p_translMat = glm::translate(glm::mat4(1.0), newPos);

				std::vector<glm::vec4> p_collider = this->shape->getCollider();
				p_collider[0] = p_translMat * p_collider[0];
				p_collider[1] = p_translMat * p_collider[1];

				if (shape::collide(p_collider, shape))
				{
					this->velocity = { 0, 0, 0 };

					if (shape->isDeadly())
						this->isDead(true);
				}
			}
		}

		// Apply translation
		if (this->velocity != glm::vec3(0, 0, 0))
			this->shape->translate(scaleMat * position);

		// Set isGrounded
		this->ground = this->velocity == glm::vec3(0, 0, 0);
	}

	void Player::applyForce(glm::vec3 force)
	{
		force *= 10000;
		this->velocity += (force / this->mass) * game::deltaTime;
	}
}