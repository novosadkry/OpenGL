#include "lua.h"

#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "render.h"
#include "shape.h"

#include <iostream>

lua_State* L;

int checkLua(int r)
{
	if (r != LUA_OK)
	{
		std::string msg = lua_tostring(L, -1);
		std::cout << "[Lua:ERROR] " << msg << std::endl;
		return 0;
	}

	return 1;
}

namespace lua
{
	void init()
	{
		L = luaL_newstate();
	}

	void close()
	{
		lua_close(L);
	}

	void LoadMaterials(std::map<std::string, Material>& materials)
	{
		if (checkLua(luaL_dofile(L, "lua/materials.lua")))
		{
			lua_getglobal(L, "materials");

			if (lua_istable(L, -1))
			{
				size_t len = lua_rawlen(L, -1);
				
				for (size_t i = 1; i <= len; i++)
				{
					std::string name;
					Material mat;

					lua_pushinteger(L, i);
					lua_gettable(L, -2);

					lua_pushstring(L, "name");
					lua_gettable(L, -2);
					name = lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_pushstring(L, "ambient");
					lua_gettable(L, -2);
					lua_rawgeti(L, -1, 3);
					lua_rawgeti(L, -2, 2);
					lua_rawgeti(L, -3, 1);
					mat.ambient.x = lua_tonumber(L, -1);
					mat.ambient.y = lua_tonumber(L, -2);
					mat.ambient.z = lua_tonumber(L, -3);
					lua_pop(L, 4);

					lua_pushstring(L, "shininess");
					lua_gettable(L, -2);
					mat.shininess = lua_tonumber(L, -1);
					lua_pop(L, 1);

					lua_pushstring(L, "diffuse");
					lua_gettable(L, -2);
					mat.diffuse = GenerateTexture(lua_tostring(L, -1));
					lua_pop(L, 1);

					lua_pushstring(L, "specular");
					lua_gettable(L, -2);
					mat.specular = GenerateTexture(lua_tostring(L, -1));
					lua_pop(L, 1);

					lua_pushstring(L, "color");
					lua_gettable(L, -2);
					lua_rawgeti(L, -1, 4);
					lua_rawgeti(L, -2, 3);
					lua_rawgeti(L, -3, 2);
					lua_rawgeti(L, -4, 1);
					mat.color.x = lua_tonumber(L, -1);
					mat.color.y = lua_tonumber(L, -2);
					mat.color.z = lua_tonumber(L, -3);
					mat.color.w = lua_tonumber(L, -4);
					lua_pop(L, 5);

					materials.emplace(name, mat);
					lua_pop(L, 1);
				}
			}

			int n = lua_gettop(L);
			lua_pop(L, n);
		}
	}

	void LoadShapes(std::map<std::string, std::unique_ptr<Shape>>& shapes)
	{
		if (checkLua(luaL_dofile(L, "lua/objects.lua")))
		{
			lua_getglobal(L, "shapes");

			if (lua_istable(L, -1))
			{
				size_t len = lua_rawlen(L, -1);

				for (size_t i = 1; i <= len; i++)
				{
					std::string name;
					std::unique_ptr<Shape> shape;

					lua_pushinteger(L, i);
					lua_gettable(L, -2);

					lua_pushstring(L, "name");
					lua_gettable(L, -2);
					name = lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_pushstring(L, "obj");
					lua_gettable(L, -2);
					shape = std::make_unique<ShapedObject>(lua_tostring(L, -1));
					lua_pop(L, 1);

					lua_pushstring(L, "material");
					lua_gettable(L, -2);
					shape->material = render::materials[lua_tostring(L, -1)];
					lua_pop(L, 1);

					lua_pushstring(L, "position");
					lua_gettable(L, -2);
					lua_rawgeti(L, -1, 3);
					lua_rawgeti(L, -2, 2);
					lua_rawgeti(L, -3, 1);
					shape->position.x = lua_tonumber(L, -1);
					shape->position.y = lua_tonumber(L, -2);
					shape->position.z = lua_tonumber(L, -3);
					lua_pop(L, 4);

					glm::vec3 euler;
					lua_pushstring(L, "rotation");
					lua_gettable(L, -2);
					lua_rawgeti(L, -1, 3);
					lua_rawgeti(L, -2, 2);
					lua_rawgeti(L, -3, 1);
					euler.x = lua_tonumber(L, -1);
					euler.y = lua_tonumber(L, -2);
					euler.z = lua_tonumber(L, -3);
					shape->rotation = glm::quat(euler);
					lua_pop(L, 4);

					lua_pushstring(L, "scale");
					lua_gettable(L, -2);
					lua_rawgeti(L, -1, 3);
					lua_rawgeti(L, -2, 2);
					lua_rawgeti(L, -3, 1);
					shape->scale.x = lua_tonumber(L, -1);
					shape->scale.y = lua_tonumber(L, -2);
					shape->scale.z = lua_tonumber(L, -3);
					lua_pop(L, 4);

					shapes.emplace(name, std::move(shape));
					lua_pop(L, 1);
				}
			}

			int n = lua_gettop(L);
			lua_pop(L, n);
		}
	}
}