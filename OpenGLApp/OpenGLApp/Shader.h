#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>

#include <GL/glew.h>

#include "StaticHelper.h"

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

class Shader
{
public:
	Shader(const char* VertexShaderPath,
			const char* FragmentShaderPath);
	~Shader();

	void CreateFromFile(const char* VertexShaderPath,
						const char* FragmentShaderPath);

	GLint GetProjectionLocation() const;
	GLint GetViewLocation() const;
	GLint GetModelLocation() const;

	GLint GetDirectionalLightSpecularIntensityLocation() const;
	GLint GetDirectionalLightShininessLocation() const;

	GLint GetUniformCameraPositionLocation() const;

	void SetDirectionalLight(const DirectionalLight& DirectionalLight) const;
	void SetPointLights(const std::vector<PointLight>& PointLights) const;
	void SetSpotLights(const std::vector<SpotLight>& SpotLights) const;

	void Use() const;
	void Clear();

	GLuint GetId() const;

private:
	GLuint Id;

	GLint UniformProjection, UniformView, UniformModel;
	
	DirectionalLightUniforms DirectionalLightUniforms;

	std::array<PointLightUniforms, StaticHelper::MAX_POINT_LIGHTS> PointLightsUniforms;
	GLint UniformPointLightsCount;

	std::array<SpotLightUniforms, StaticHelper::MAX_SPOT_LIGHTS> SpotLightsUniforms;
	GLint UniformSpotLightsCount;

	GLint UniformMaterialSpecularIntensity;
	GLint UniformMaterialShininess;

	GLint UniformCameraPosition;


	void CompileShaders(const char* VertexShaderCode,
						const char* FragmentShaderCode);

	/*  1) Created shader from shader source code
		2) Compile it and check compilation status
		3) Attach it to shader program				*/
	void AddShader(GLuint ShaderProgramId, const char* ShaderCode, GLenum ShaderType) const;

	std::string ReadFile(const char* FilePath);
};

