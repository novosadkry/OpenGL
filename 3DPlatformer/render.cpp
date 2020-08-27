#include "render.h"

namespace render
{
	Camera cam = Camera();
	std::vector<Light> lights = std::vector<Light>();
	std::vector<Shape*> shapes = std::vector<Shape*>();

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
}