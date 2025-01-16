#include "PointLight.h"

#include <gtc/type_ptr.hpp>

PointLight::PointLight(glm::vec4 InitColor,
						GLfloat InitAmbientIntensity,
						GLfloat InitDiffuseIntensity,
						glm::vec3 InitPosition,
						GLfloat InitExponent,
						GLfloat InitLinear,
						GLfloat InitConstant)
{
	Color = InitColor;
	AmbientIntensity = InitAmbientIntensity;
	DiffuseIntensity = InitDiffuseIntensity;

	Position = InitPosition;
	Exponent = InitExponent;
	Linear = InitLinear;
	Constant = InitConstant;
}

void PointLight::Apply(GLint ColorLocation,
						GLint AmbientIntensityLocation,
						GLint DiffuseIntensityLocation,
						GLint PositionLocation,
						GLint ExponentLocation,
						GLint LinearLocation,
						GLint ConstantLocation) const
{
	glUniform4fv(ColorLocation, 1, glm::value_ptr(Color));
	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform1f(DiffuseIntensityLocation, DiffuseIntensity);
	glUniform3fv(PositionLocation, 1, glm::value_ptr(Position));
	glUniform1f(ExponentLocation, Exponent);
	glUniform1f(LinearLocation, Linear);
	glUniform1f(ConstantLocation, Constant);
}