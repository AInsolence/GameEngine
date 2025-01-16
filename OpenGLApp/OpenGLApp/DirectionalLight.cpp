#include "DirectionalLight.h"

#include <gtc/type_ptr.hpp>

DirectionalLight::DirectionalLight(glm::vec4 InitColor,
									GLfloat InitAmbientIntensity,
									GLfloat InitDiffuseIntensity,
									glm::vec3 InitDirection)
{
	Color = InitColor;
	AmbientIntensity = InitAmbientIntensity;
	DiffuseIntensity = InitDiffuseIntensity;
	Direction = InitDirection;
}

void DirectionalLight::Apply(GLint ColorLocation,
							GLint AmbientIntensityLocation,
							GLint DiffuseIntensityLocation,
							GLint DirectionLocation) const
{
	glUniform4fv(ColorLocation, 1, glm::value_ptr(Color));
	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform1f(DiffuseIntensityLocation, DiffuseIntensity);
	glUniform3fv(DirectionLocation, 1, glm::value_ptr(Direction));
}
