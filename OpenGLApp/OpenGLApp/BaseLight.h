#pragma once

#include <glm.hpp>
#include <GL/glew.h>

class BaseLight
{
public:
	BaseLight();

protected:
	glm::vec4 Color;

	GLfloat AmbientIntensity;
	GLfloat DiffuseIntensity;
};
