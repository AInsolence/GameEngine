#pragma once

#include <memory>
#include <GL/glew.h>

#include "vec4.hpp"
#include "mat4x4.hpp"

#include "ShadowMap.h"

struct BaseLightParams
{
	glm::vec4 InitColor {1.0f};
	GLfloat InitAmbientIntensity = 0.1f;
	GLfloat InitDiffuseIntensity = 1.0f;
	GLint InitShadowWidth = 1024;
	GLint InitShadowHeight = 1024;
};

class BaseLight
{
public:
	BaseLight(glm::vec4 InitColor,
				GLfloat InitAmbientIntensity,
				GLfloat InitDiffuseIntensity,
				GLint InitShadowWidth,
				GLint InitShadowHeight);

	const std::shared_ptr<ShadowMap>& GetShadowMap() const { return ShadowMapTex; }

protected:
	glm::vec4 Color;

	GLfloat AmbientIntensity;
	GLfloat DiffuseIntensity;

	glm::mat4 LightProjectionMatrix;

	std::shared_ptr<ShadowMap> ShadowMapTex;
};
