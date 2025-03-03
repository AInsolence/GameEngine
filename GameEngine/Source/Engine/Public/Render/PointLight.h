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
				GLfloat InitRadiusSharpness = 5.0f,
				GLint InitShadowWidth = 1024,
				GLint InitShadowHeight = 1024,
				GLfloat InitNear = 0.01f,
				GLfloat InitFar = 100.0f);

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

	std::array<glm::mat4, 6> CalculateLightSpaceTransform();

	// Getters
	glm::vec3 GetPosition() const { return Position; }
	GLfloat GetExponent() const { return Exponent; }
	GLfloat GetLinear() const { return Linear; }
	GLfloat GetConstant() const { return Constant; }
	GLfloat GetInnerRadius() const { return InnerRadius; }
	GLfloat GetOuterRadius() const { return OuterRadius; }
	GLfloat GetRadiusSharpness() const { return RadiusSharpness; }
	GLfloat GetNear() const { return Near; }
	GLfloat GetFarPlane() const { return Far; }

	// Setters
	void SetPosition(glm::vec3&& InPosition) { Position = InPosition; }
	void SetExponent(GLfloat InExponent) { Exponent = InExponent; }
	void SetLinear(GLfloat InLinear) { Linear = InLinear; }
	void SetConstant(GLfloat InConstant) { Constant = InConstant; }
	void SetInnerRadius(GLfloat InInnerRadius) { InnerRadius = InInnerRadius; }
	void SetOuterRadius(GLfloat InOuterRadius) { OuterRadius = InOuterRadius; }
	void SetRadiusSharpness(GLfloat InRadiusSharpness) { RadiusSharpness = InRadiusSharpness; }
	void SetNear(GLfloat InNear) { Near = InNear; }
	void SetFar(GLfloat InFar) { Far = InFar; }

protected:
	glm::vec3 Position;

	GLfloat Exponent, Linear, Constant;
	GLfloat InnerRadius, OuterRadius, RadiusSharpness;

	GLfloat Near;
	GLfloat Far;
};
