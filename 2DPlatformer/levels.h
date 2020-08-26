#pragma once

#include <vector>

typedef void (*Level)();

namespace game
{
	extern std::vector<Level>* levels;
}