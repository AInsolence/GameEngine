#pragma once

#include <glm.hpp>
#include <memory>
#include <GL/glew.h>

#include "ShadowMap.h"

class BaseLight
{
public:
	BaseLight(glm::vec4 InitColor,
				GLfloat InitAmbientIntensity,
				GLfloat InitDiffuseIntensity,
				GLint InitShadowWidth = 32,
				GLint InitShadowHeight = 32);

	const std::unique_ptr<ShadowMap>& GetShadowMap() const { return ShadowMapTex; }

protected:
	glm::vec4 Color;

	GLfloat AmbientIntensity;
	GLfloat DiffuseIntensity;

	glm::mat4 LightProjectionMatrix;

	std::unique_ptr<ShadowMap> ShadowMapTex;
};
