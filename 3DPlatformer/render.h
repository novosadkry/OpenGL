#pragma once

#include "shape.h"

#include <vector>
#include <string>
#include <map>

namespace render
{
	extern Camera cam;
	extern std::vector<Light> lights;
	extern std::vector<Shape*> shapes;
	extern std::map<std::string, Material> materials;

	extern float lastFrame;
	extern float deltaTime;
}