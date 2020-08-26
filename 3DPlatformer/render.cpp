#include "render.h"

namespace render
{
	Camera* cam = nullptr;
	Light* light = nullptr;

	std::vector<Shape*>* shapes = nullptr;

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
}