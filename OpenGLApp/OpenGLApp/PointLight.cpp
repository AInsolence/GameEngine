#include "PointLight.h"

#include <gtc/type_ptr.hpp>

PointLight::PointLight(glm::vec4 InitColor,
						GLfloat InitAmbientIntensity,
						GLfloat InitDiffuseIntensity,
						glm::vec3 InitPosition,
						GLfloat InitExponent,
						GLfloat InitLinear,
						GLfloat InitConstant,
						GLfloat InitInnerRadius,
						GLfloat InitOuterRadius,
						GLfloat InitRadiusSharpness)
{
	Color = InitColor;
	AmbientIntensity = InitAmbientIntensity;
	DiffuseIntensity = InitDiffuseIntensity;

	Position = InitPosition;
	Exponent = InitExponent;
	Linear = InitLinear;
	Constant = InitConstant;

	InnerRadius = InitInnerRadius;
	OuterRadius = InitOuterRadius;
	RadiusSharpness = InitRadiusSharpness;
}

void PointLight::Apply(GLint ColorLocation,
						GLint AmbientIntensityLocation,
						GLint DiffuseIntensityLocation,
						GLint PositionLocation,
						GLint ExponentLocation,
						GLint LinearLocation,
						GLint ConstantLocation,
						GLint InnerRadiusLocation,
						GLint OuterRadiusLocation,
						GLint RadiusSharpnessLocation) const
{
	glUniform4fv(ColorLocation, 1, glm::value_ptr(Color));

	glUniform1f(AmbientIntensityLocation, AmbientIntensity);
	glUniform1f(DiffuseIntensityLocation, DiffuseIntensity);

	glUniform3fv(PositionLocation, 1, glm::value_ptr(Position));

	glUniform1f(ExponentLocation, Exponent);
	glUniform1f(LinearLocation, Linear);
	glUniform1f(ConstantLocation, Constant);

	glUniform1f(InnerRadiusLocation, InnerRadius);
	glUniform1f(OuterRadiusLocation, OuterRadius);
	glUniform1f(RadiusSharpnessLocation, RadiusSharpness);
}