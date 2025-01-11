#include "BaseLight.h"

#include <gtc/type_ptr.hpp>

BaseLight::BaseLight(glm::vec4 InitColor, GLfloat InitIntensity)
{
	Color = InitColor;
	Intensity = InitIntensity;
}

BaseLight::~BaseLight()
{
}

void BaseLight::Apply(GLint ColorLocation, GLint IntensityLocation)
{
	glUniform4fv(ColorLocation, 1, glm::value_ptr(Color));
	glUniform1f(IntensityLocation, Intensity);
}
