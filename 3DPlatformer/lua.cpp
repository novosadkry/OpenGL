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

	glm::vec3 getVec3(const char* s)
	{
		glm::vec3 v;

		lua_pushstring(L, s);
		lua_gettable(L, -2);
		lua_rawgeti(L, -1, 3);
		lua_rawgeti(L, -2, 2);
		lua_rawgeti(L, -3, 1);
		v.x = lua_tonumber(L, -1);
		v.y = lua_tonumber(L, -2);
		v.z = lua_tonumber(L, -3);
		lua_pop(L, 4);

		return v;
	}

	glm::vec4 getVec4(const char* s)
	{
		glm::vec4 v;

		lua_pushstring(L, s);
		lua_gettable(L, -2);
		lua_rawgeti(L, -1, 4);
		lua_rawgeti(L, -2, 3);
		lua_rawgeti(L, -3, 2);
		lua_rawgeti(L, -4, 1);
		v.x = lua_tonumber(L, -1);
		v.y = lua_tonumber(L, -2);
		v.z = lua_tonumber(L, -3);
		v.w = lua_tonumber(L, -4);
		lua_pop(L, 5);

		return v;
	}

	std::string getString(const char* s)
	{
		std::string _s;

		lua_pushstring(L, s);
		lua_gettable(L, -2);
		_s = lua_tostring(L, -1);
		lua_pop(L, 1);

		return _s;
	}

	float getFloat(const char* s)
	{
		float f;

		lua_pushstring(L, s);
		lua_gettable(L, -2);
		f = lua_tonumber(L, -1);
		lua_pop(L, 1);

		return f;
	}

	GLuint getTexture(const char* s)
	{
		GLuint id;

		lua_pushstring(L, s);
		lua_gettable(L, -2);
		id = GenerateTexture(lua_tostring(L, -1));
		lua_pop(L, 1);

		return id;
	}

	void LoadMaterials(std::map<std::string, Material>& materials)
	{
		if (checkLua(luaL_dofile(L, "lua/materials.lua")))
		{
			lua_getglobal(L, "materials");
			lua_pushnil(L);

			while (lua_next(L, -2))
			{
				std::string name;
				Material mat;

				name = lua_tostring(L, -2);
				mat.ambient = getVec3("ambient");
				mat.shininess = getFloat("shininess");
				mat.diffuse = getTexture("diffuse");
				mat.specular = getTexture("specular");
				mat.color = getVec4("color");

				materials.emplace(name, mat);
				lua_pop(L, 1);
			}

			lua_pop(L, lua_gettop(L));
		}
	}

	void LoadShapes(std::map<std::string, std::shared_ptr<Shape>>& shapes, std::map<std::string, Light>& lights)
	{
		if (checkLua(luaL_dofile(L, "lua/shapes.lua")))
		{
			lua_getglobal(L, "shapes");
			lua_pushnil(L);

			while (lua_next(L, -2))
			{
				std::string name;
				std::shared_ptr<Shape> shape;

				name = lua_tostring(L, -2);
				shape = std::make_shared<ShapedObject>(getString("obj").c_str());
				shape->material = render::materials[getString("material")];
				shape->position = getVec3("position");
				shape->scale = getVec3("scale");
				shape->rotation = getVec3("rotation");

				shapes.emplace(name, std::move(shape));
				lua_pop(L, 1);
			}

			lua_pop(L, 1);

			lua_getglobal(L, "lights");
			lua_pushnil(L);

			while (lua_next(L, -2))
			{
				std::string name;
				Light light;

				name = lua_tostring(L, -2);
				light.position = getVec3("position");
				light.color = getVec3("color");
				light.intensity = getFloat("intensity");

				lights.emplace(name, light);
				lua_pop(L, 1);
			}

			lua_pop(L, lua_gettop(L));
		}
	}
}