#pragma once
#include "BaseLight.h"

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight(glm::vec4 InitColor,
					GLfloat InitAmbientIntensity,
					GLfloat InitDiffuseIntensity,
					glm::vec3 InitDirection,
					GLint InitShadowWidth,
					GLint InitShadowHeight);

	void Apply (GLint ColorLocation,
				GLint AmbientIntensityLocation,
				GLint DiffuseIntensityLocation,
				GLint DirectionLocation) const;

	glm::mat4 CalculateLightSpaceTransform();

private:
	glm::vec3 Direction;
};

