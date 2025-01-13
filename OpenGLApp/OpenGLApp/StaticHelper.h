#pragma once

#include <GL/glew.h>

#include <vector>

class StaticHelper
{
public:
	static std::vector<GLenum> EnsureGLFunction(const char* FunctionName);
	static void CalculateAverageNormals(const unsigned int* Indices,
										unsigned int IndexCount,
										GLfloat* Vertices,
										unsigned int VertexCount,
										unsigned int VerticesOffset,
										unsigned int NormalsOffset);
};

