#include "PointLight.h"

#include <array>
#include <gtc/type_ptr.hpp>

#include "OmniDirShadowMap.h"

PointLight::PointLight(glm::vec4 InitColor,
						GLfloat InitAmbientIntensity,
						GLfloat InitDiffuseIntensity,
						glm::vec3 InitPosition,
						GLfloat InitExponent,
						GLfloat InitLinear,
						GLfloat InitConstant,
						GLfloat InitInnerRadius,
						GLfloat InitOuterRadius,
						GLfloat InitRadiusSharpness,
						GLint InitShadowWidth,
						GLint InitShadowHeight,
						GLfloat InitNear,
						GLfloat InitFar)
										: BaseLight(InitColor,
													InitAmbientIntensity,
													InitDiffuseIntensity,
													InitShadowWidth,
													InitShadowHeight)
{
	Position = InitPosition;
	Exponent = InitExponent;
	Linear = InitLinear;
	Constant = InitConstant;

	InnerRadius = InitInnerRadius;
	OuterRadius = InitOuterRadius;
	RadiusSharpness = InitRadiusSharpness;

	Near = InitNear;
	Far = InitFar;

	float Aspect = static_cast<float>(InitShadowWidth) / 
						static_cast<float>(InitShadowHeight);

	LightProjectionMatrix = glm::perspective(glm::radians(90.0f), Aspect, Near, Far);

	ShadowMapTex = std::make_unique<OmniDirShadowMap>();
	ShadowMapTex->Init(InitShadowWidth, InitShadowHeight);
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

std::array<glm::mat4, 6> PointLight::CalculateLightSpaceTransform()
{
	std::array<glm::mat4, 6> LightMatrices;

	// +x -x
	LightMatrices.at(0) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f));

	LightMatrices.at(1) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(-1.0f, 0.0f, 0.0f),
															glm::vec3(0.0f, -1.0f, 0.0f));
	// +y -y
	LightMatrices.at(2) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(0.0f, 1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, 1.0f));

	LightMatrices.at(3) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(0.0f, -1.0f, 0.0f),
															glm::vec3(0.0f, 0.0f, -1.0f));
	// +z -z
	LightMatrices.at(4) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(0.0f, 0.0f, 1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f));

	LightMatrices.at(5) = LightProjectionMatrix * 
								glm::lookAt(Position,
											Position + glm::vec3(0.0f, 0.0f, -1.0f),
															glm::vec3(0.0f, -1.0f, 0.0f));

	return LightMatrices;
}
