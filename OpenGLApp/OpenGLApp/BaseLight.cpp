#include "BaseLight.h"

BaseLight::BaseLight()
{
	Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	AmbientIntensity = 1.0f;
	DiffuseIntensity = 1.0f;
	Direction = glm::vec3(1.0f, 1.0f, 1.0f);
}
