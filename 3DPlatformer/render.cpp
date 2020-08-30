#include "render.h"

namespace render
{
	Camera cam;
	std::vector<Light> lights;
	std::vector<Shape*> shapes;
	std::map<std::string, Material> materials;

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
}