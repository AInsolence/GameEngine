#pragma once

#include <memory>
#include <GL/glew.h>

#include <vector>
#include "Components/Texture.h"


static const auto PlaceholderTexture = std::make_shared<Texture>("Content/Textures/plain.png");

class Helper
{
public:
	static constexpr int MAX_POINT_LIGHTS = 10;
	static constexpr int MAX_SPOT_LIGHTS = 10;

	static std::vector<GLenum> EnsureGL(const char* GL_FunctionName,
											const char* File,
											unsigned int Line);

	static void CalculateAverageNormals(const std::vector <unsigned int>& Indices,
										std::vector<GLfloat>& Vertices,
										unsigned int VerticesOffset,
										unsigned int NormalsOffset);
};
