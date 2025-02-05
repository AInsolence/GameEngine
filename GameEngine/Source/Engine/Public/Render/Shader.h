#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <fwd.hpp>

#include <GL/glew.h>

#include "Helper.h"

class SpotLight;
class PointLight;
class DirectionalLight;

struct DirectionalLightUniforms
{
	GLint Color;
	GLint AmbientIntensity;
	GLint DiffuseIntensity;
	GLint Direction;
};

struct PointLightUniforms
{
	GLint Color;
	GLint AmbientIntensity;
	GLint DiffuseIntensity;
	GLint Position;

	GLint Exponent;
	GLint Linear;
	GLint Constant;

	GLint InnerRadius;
	GLint OuterRadius;
	GLint RadiusSharpness;
};

struct SpotLightUniforms
{
	GLint Color;
	GLint AmbientIntensity;
	GLint DiffuseIntensity;
	GLint Position;
	GLint Direction;

	GLint CutOffAngleCos;

	GLint Exponent;
	GLint Linear;
	GLint Constant;

	GLint InnerRadius;
	GLint OuterRadius;
	GLint RadiusSharpness;
};

struct OmniShadowUniforms
{
	GLint ShadowMap;
	GLint FarPlane;
};

class Shader
{
public:
	Shader(const char* VertexShaderPath,
			const char* FragmentShaderPath);

	Shader(const char* VertexShaderPath,
			const char* GeometryShaderPath,
			const char* FragmentShaderPath);

	~Shader();

	void CreateFromFile(const char* VertexShaderPath,
						const char* FragmentShaderPath);

	void CreateFromFile(const char* VertexShaderPath,
						const char* GeometryShaderPath,
						const char* FragmentShaderPath);

	GLint GetProjectionLocation() const;
	GLint GetViewLocation() const;
	GLint GetModelLocation() const;

	GLint GetDirectionalLightSpecularIntensityLocation() const;
	GLint GetDirectionalLightShininessLocation() const;

	GLint GetUniformCameraPositionLocation() const;
	GLint GetUniformOmniLightPositionLocation() const;
	GLint GetUniformFarPlaneLocation() const;
	GLint GetGridColorLocation() const;

	void SetDirectionalLight(const std::shared_ptr<DirectionalLight>& DirectionalLight) const;

	void SetPointLights(const std::vector<std::shared_ptr<PointLight>>& PointLights,
						unsigned int StartUnit,
						unsigned int Offset) const;

	void SetSpotLights(const std::vector<std::shared_ptr<SpotLight>>& SpotLights,
						unsigned int StartUnit,
						unsigned int Offset) const;

	void SetTextureUnit(GLint TextureUnit) const;
	void SetSkyboxTextureUnit(GLint TextureUnit) const;
	void SetDirectionalShadowMap(GLint TextureUnit) const;
	void SetDirectionalLightSpaceTransform(const glm::mat4& Transform) const;

	void SetLightMatrices(const std::array<glm::mat4, 6>& LightMatrices) const;

	void Validate() const;
	void Use() const;
	void Clear();

	GLuint GetId() const;

private:
	GLuint Id;

	GLint UniformProjection, UniformView, UniformModel;
	
	DirectionalLightUniforms DirectionalLightUniforms;

	std::array<PointLightUniforms, Helper::MAX_POINT_LIGHTS> PointLightsUniforms;
	GLint UniformPointLightsCount;

	std::array<SpotLightUniforms, Helper::MAX_SPOT_LIGHTS> SpotLightsUniforms;
	GLint UniformSpotLightsCount;

	std::array<OmniShadowUniforms, Helper::MAX_POINT_LIGHTS + Helper::MAX_SPOT_LIGHTS> OmniShadowMapsUniforms;

	GLint UniformMaterialSpecularIntensity;
	GLint UniformMaterialShininess;

	GLint UniformCameraPosition;
	GLint UniformSkybox;

	// Shadows
	GLint UniformTexture;
	GLint UniformDirectionalShadowMap;
	GLint UniformDirectionalLightSpaceTransform;

	GLint UniformOmniLightPosition;
	GLint UniformFarPlane;
	std::array<GLint, 6> UniformLightMatrices;

	GLint UniformGridColor;

	void CompileProgram();

	void CompileShaders(const char* VertexShaderCode,
						const char* FragmentShaderCode);

	void CompileShaders(const char* VertexShaderCode,
						const char* GeometryShaderCode,
						const char* FragmentShaderCode);

	/*  1) Created shader from shader source code
		2) Compile it and check compilation status
		3) Attach it to shader program				*/
	void AddShader(GLuint ShaderProgramId, const char* ShaderCode, GLenum ShaderType) const;

	std::string ReadFile(const char* FilePath);
};

