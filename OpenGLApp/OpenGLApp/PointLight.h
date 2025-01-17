#pragma once
#include "BaseLight.h"
class PointLight : public BaseLight
{
public:
	PointLight(glm::vec4 InitColor,
				GLfloat InitAmbientIntensity,
				GLfloat InitDiffuseIntensity,
				glm::vec3 InitPosition,
				GLfloat InitExponent,
				GLfloat InitLinear,
				GLfloat InitConstant = 1.0f,
				GLfloat InitInnerRadius = 1.0f,
				GLfloat InitOuterRadius = 1.0f,
				GLfloat InitRadiusSharpness = 5.0f);

	void Apply(GLint ColorLocation,
				GLint AmbientIntensityLocation,
				GLint DiffuseIntensityLocation,
				GLint PositionLocation,
				GLint ExponentLocation,
				GLint LinearLocation,
				GLint ConstantLocation,
				GLint InnerRadiusLocation,
				GLint OuterRadiusLocation,
				GLint RadiusSharpnessLocation) const;
private:
	glm::vec3 Position;
	GLfloat Exponent, Linear, Constant;
	GLfloat InnerRadius, OuterRadius, RadiusSharpness;
};
