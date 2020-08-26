#pragma once

#include "shape.h"

#include <vector>

namespace render
{
	extern Camera* cam;
	extern Light* light;

	extern std::vector<Shape*>* shapes;

	extern float lastFrame;
	extern float deltaTime;
}