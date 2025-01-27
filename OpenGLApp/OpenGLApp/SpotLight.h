#pragma once
#include "PointLight.h"
class SpotLight : public PointLight
{
public:
	SpotLight(glm::vec4 InitColor,
				GLfloat InitAmbientIntensity,
				GLfloat InitDiffuseIntensity,
				glm::vec3 InitPosition,
				glm::vec3 InitDirection,
				GLfloat InitCutOffAngle,
				GLfloat InitExponent,
				GLfloat InitLinear,
				GLfloat InitConstant = 1.0f,
				GLfloat InitInnerRadius = 1.0f,
				GLfloat InitOuterRadius = 1.0f,
				GLfloat InitRadiusSharpness = 5.0f,
				GLint InitShadowWidth = 1024,
				GLint InitShadowHeight = 1024,
				GLfloat Near = 0,
				GLfloat Far = 50);

	void Apply(GLint ColorLocation,
				GLint AmbientIntensityLocation,
				GLint DiffuseIntensityLocation,
				GLint PositionLocation,
				GLint DirectionLocation,
				GLint CutOffAngleCosLocation,
				GLint ExponentLocation,
				GLint LinearLocation,
				GLint ConstantLocation,
				GLint InnerRadiusLocation,
				GLint OuterRadiusLocation,
				GLint RadiusSharpnessLocation) const;

	void SetTransform(glm::vec3 InPosition, glm::vec3 InDirection);

	// TODO only for debug. Should be moved to Controller from Renderer
	void Toggle();
	bool isOn = false;
	
protected:
	glm::vec3 Direction;
	float CutOffAngleCos;
};
