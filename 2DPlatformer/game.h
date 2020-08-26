#pragma once

#include "glm\glm.hpp"

#include "shape.h"
#include "levels.h"
#include "player.h"

namespace game
{
	extern float deltaTime;
	extern float air;
	extern glm::vec3 gravity;

	extern player::Player* player;
	extern std::vector<shape::Shape*>* shapes;

	extern unsigned int currentLevel;
	extern bool levelComplete;

	extern shape::Shape* entry;
	extern shape::Shape* exit;

	void tick();

	void restartLevel();
	void nextLevel();

	void purge();
}