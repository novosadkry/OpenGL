#pragma once

#include "shape.h"
#include "object.h"

#include <vector>
#include <memory>
#include <string>
#include <map>

namespace render
{
	extern Camera cam;
	extern std::vector<Light> lights;
	extern std::map<std::string, std::unique_ptr<Shape>> shapes;
	extern std::map<std::string, Object> objects;
	extern std::map<std::string, Material> materials;

	extern float lastFrame;
	extern float deltaTime;
}