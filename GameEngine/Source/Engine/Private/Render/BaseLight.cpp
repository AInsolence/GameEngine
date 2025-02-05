#include "Render/BaseLight.h"

#include <ext/matrix_clip_space.hpp>

BaseLight::BaseLight(glm::vec4 InitColor,
					GLfloat InitAmbientIntensity,
					GLfloat InitDiffuseIntensity,
					GLint InitShadowWidth,
					GLint InitShadowHeight)
{
	Color = InitColor;
	AmbientIntensity = InitAmbientIntensity;
	DiffuseIntensity = InitDiffuseIntensity;

	ShadowMapTex = std::make_shared<ShadowMap>();
	ShadowMapTex->Init(InitShadowWidth, InitShadowHeight);

	LightProjectionMatrix = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 100.0f);
}
