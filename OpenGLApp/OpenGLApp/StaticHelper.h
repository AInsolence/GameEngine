#pragma once

#include <GL/glew.h>

#include <vector>

class StaticHelper
{
public:
	static std::vector<GLenum> EnsureGLFunction(const char* FunctionName);

};

