#pragma once

#include <memory>
#include <GL/glew.h>

#include <vector>
#include "Texture.h"

#define ENSURE_GL(GL_FunctionName) StaticHelper::EnsureGLFunction(GL_FunctionName)

static const auto PlaceholderTexture = std::make_shared<Texture>("Content/Textures/plain.png");

class StaticHelper
{
public:
	static constexpr int MAX_POINT_LIGHTS = 10;
	static constexpr int MAX_SPOT_LIGHTS = 10;

	static std::vector<GLenum> EnsureGLFunction(const char* GL_FunctionName);

	static void CalculateAverageNormals(const std::vector <unsigned int>& Indices,
										std::vector<GLfloat>& Vertices,
										unsigned int VerticesOffset,
										unsigned int NormalsOffset);
};
