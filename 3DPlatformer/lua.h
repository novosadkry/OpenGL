#pragma once
#pragma comment(lib, "liblua54.a")

#include <lua.hpp>
#include <lualib.h>

#include "material.h"

#include <string>
#include <map>

namespace lua
{
	void init();
	void close();

	void LoadMaterials(std::map<std::string, Material>& materials);
}