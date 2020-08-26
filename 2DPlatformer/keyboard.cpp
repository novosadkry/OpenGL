#include <iostream>
#include <glfw3.h>

#include "player.h"
#include "game.h"

void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::vec3 force = glm::vec3();

	if (game::player->isGrounded())
	{
		bool bKeyA = (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT));
		bool bKeyD = (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT));
		bool bKeyW = (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP));

		force.x -= bKeyA;
		force.x += bKeyD;

		force.y += bKeyA || bKeyD || bKeyW;
		force.y *= 2.0f;
	}

	else
	{
		bool bKeyA = (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) 
			&& action == GLFW_PRESS
			&& game::player->getVelocity().x >= 0;

		bool bKeyD = (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT))
			&& action == GLFW_PRESS
			&& game::player->getVelocity().x <= 0;

		force.x -= bKeyA;
		force.x += bKeyD;
	}

	if (force.x != 0 || force.y != 0 || force.z != 0)
		game::player->applyForce(force * game::deltaTime);

	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}