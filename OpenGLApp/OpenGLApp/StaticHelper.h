#pragma once

#include <GL/glew.h>

#include <vector>

class StaticHelper
{
public:

	static constexpr int MAX_POINT_LIGHTS = 10;
	static constexpr int MAX_SPOT_LIGHTS = 10;

	static std::vector<GLenum> EnsureGLFunction(const char* FunctionName);

	static void CalculateAverageNormals(const std::vector <unsigned int>& Indices,
										std::vector<GLfloat>& Vertices,
										unsigned int VerticesOffset,
										unsigned int NormalsOffset);
};
