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
	glm::vec3 GetDirection() const { return Direction; }
	void SetDirection(glm::vec3&& InDirection) { Direction = InDirection; }

private:
	glm::vec3 Direction;
};

