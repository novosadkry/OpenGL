#include "game.h"

#include <glfw3.h>

namespace game
{
	float deltaTime = 0.0f;
	float air = 0.0f;
	glm::vec3 gravity;

	player::Player* player = nullptr;
	std::vector<shape::Shape*>* shapes = nullptr;

	unsigned int currentLevel = 0;
	bool levelComplete = false;

	shape::Shape* entry = nullptr;
	shape::Shape* exit = nullptr;

	void tick()
	{
		static float lastTick = 0.0f;
		float thisTick = glfwGetTime();

		deltaTime = thisTick - lastTick;
		lastTick = thisTick;

		player->move();

		if (player->getShape()->collide(exit))
			levelComplete = true;
	}

	void restartLevel()
	{
		purge();

		levels->at(game::currentLevel)();
		levelComplete = false;
	}

	void nextLevel()
	{
		currentLevel += 1;

		if (currentLevel < levels->size())
			restartLevel();
		else
			assert(0);
	}

	void purge()
	{
		for (shape::Shape* shape : *game::shapes)
			delete shape;

		delete game::shapes;

		delete game::player;
		//delete game::entry;
		//delete game::exit;
	}
}