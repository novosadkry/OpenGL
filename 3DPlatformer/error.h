#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) glClearError();\
	x;\
	ASSERT(glLogError(#x, __FILE__, __LINE__))

void glClearError();

int glLogError(const char* function, const char* file, int line);