#include "render.h"

namespace render
{
	Camera cam;
	std::map<std::string, std::shared_ptr<Shape>> shapes;
	std::map<std::string, Light> lights;
	std::map<std::string, Object> objects;
	std::map<std::string, Material> materials;

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
}