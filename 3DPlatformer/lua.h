#pragma once
#pragma comment(lib, "liblua54.a")

#include <lua.hpp>
#include <lualib.h>

#include "material.h"
#include "shape.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace lua
{
	void init();
	void close();

	void LoadMaterials(std::map<std::string, Material>& materials);
	void LoadShapes(std::map<std::string, std::shared_ptr<Shape>>& shapes, std::map<std::string, Light>& lights);
}