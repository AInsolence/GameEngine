#pragma once

#include <GL/glew.h>

#include <vector>

class StaticHelper
{
public:
	static std::vector<GLenum> EnsureGLFunction(const char* FunctionName);
	static void CalculateAverageNormals(unsigned int* Indices,
										unsigned int IndiceCount,
										GLfloat* Vertices,
										unsigned int VerticesCount,
										unsigned int VerticesOffset,
										unsigned int NormalsOffset);
};

