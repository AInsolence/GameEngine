#pragma once

#include <glm.hpp>
#include <GL/glew.h>

class BaseLight
{
public:
	explicit BaseLight(glm::vec4 InitColor, GLfloat InitIntensity);
	~BaseLight();

	void Apply(GLint ColorLocation, GLint IntensityLocation);

private:
	glm::vec4 Color;
	GLfloat Intensity;
};

