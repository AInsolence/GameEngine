#include "Shader.h"

#include <iostream>
#include <sstream>
#include <gtc/type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Helper.h"

Shader::Shader(const char* VertexShaderPath,
				const char* FragmentShaderPath)
{
	Id = 0;
	UniformModel = 0;
	UniformProjection = 0;

	CreateFromFile(VertexShaderPath, FragmentShaderPath);
}

Shader::Shader(const char* VertexShaderPath,
				const char* GeometryShaderPath,
				const char* FragmentShaderPath)
{
	Id = 0;
	UniformModel = 0;
	UniformProjection = 0;

	CreateFromFile(VertexShaderPath, GeometryShaderPath, FragmentShaderPath);
}

Shader::~Shader()
{
	Clear();
}

void Shader::CreateFromFile(const char* VertexShaderPath,
							const char* GeometryShaderPath,
							const char* FragmentShaderPath)
{
	const std::string VertexShaderCode = ReadFile(VertexShaderPath);
	const std::string GeometryShaderCode = ReadFile(GeometryShaderPath);
	const std::string FragmentShaderCode = ReadFile(FragmentShaderPath);

	CompileShaders(VertexShaderCode.c_str(),
					GeometryShaderCode.c_str(),
					FragmentShaderCode.c_str());
}

void Shader::CreateFromFile(const char* VertexShaderPath, const char* FragmentShaderPath)
{
	const std::string VertexShaderCode = ReadFile(VertexShaderPath);
	const std::string FragmentShaderCode = ReadFile(FragmentShaderPath);

	CompileShaders(VertexShaderCode.c_str(),
					FragmentShaderCode.c_str());
}

GLint Shader::GetProjectionLocation() const
{
	return UniformProjection;
}

GLint Shader::GetViewLocation() const
{
	return UniformView;
}

GLint Shader::GetModelLocation() const
{
	return UniformModel;
}

GLint Shader::GetDirectionalLightSpecularIntensityLocation() const
{
	return UniformMaterialSpecularIntensity;
}

GLint Shader::GetDirectionalLightShininessLocation() const
{
	return UniformMaterialShininess;
}

GLint Shader::GetUniformCameraPositionLocation() const
{
	return UniformCameraPosition;
}

GLint Shader::GetUniformOmniLightPositionLocation() const
{
	return UniformOmniLightPosition;
}

GLint Shader::GetUniformFarPlaneLocation() const
{
	return UniformFarPlane;
}

void Shader::SetDirectionalLight(const DirectionalLight& DirectionalLight) const
{
	DirectionalLight.Apply(DirectionalLightUniforms.Color,
							DirectionalLightUniforms.AmbientIntensity,
							DirectionalLightUniforms.DiffuseIntensity,
							DirectionalLightUniforms.Direction);
}

void Shader::SetPointLights(const std::vector<PointLight>& PointLights) const
{
	const int PointLightsCount = PointLights.size() > Helper::MAX_POINT_LIGHTS
											? Helper::MAX_POINT_LIGHTS
											: PointLights.size();

	glUniform1i(UniformPointLightsCount, PointLightsCount);

	for (int PointLightIndex = 0; PointLightIndex < PointLightsCount; PointLightIndex++)
	{
		PointLights[PointLightIndex].Apply(PointLightsUniforms[PointLightIndex].Color,
											PointLightsUniforms[PointLightIndex].AmbientIntensity,
											PointLightsUniforms[PointLightIndex].DiffuseIntensity,
											PointLightsUniforms[PointLightIndex].Position,
											PointLightsUniforms[PointLightIndex].Exponent,
											PointLightsUniforms[PointLightIndex].Linear,
											PointLightsUniforms[PointLightIndex].Constant,
											PointLightsUniforms[PointLightIndex].InnerRadius,
											PointLightsUniforms[PointLightIndex].OuterRadius,
											PointLightsUniforms[PointLightIndex].RadiusSharpness);
	}
}

void Shader::SetSpotLights(const std::vector<SpotLight>& SpotLights) const
{
	const int SpotLightsCount = SpotLights.size() > Helper::MAX_SPOT_LIGHTS
											? Helper::MAX_SPOT_LIGHTS
											: SpotLights.size();

	glUniform1i(UniformSpotLightsCount, SpotLightsCount);

	for (int SpotLightIndex = 0; SpotLightIndex < SpotLightsCount; SpotLightIndex++)
	{
		SpotLights[SpotLightIndex].Apply(SpotLightsUniforms[SpotLightIndex].Color,
											SpotLightsUniforms[SpotLightIndex].AmbientIntensity,
											SpotLightsUniforms[SpotLightIndex].DiffuseIntensity,
											SpotLightsUniforms[SpotLightIndex].Position,
											SpotLightsUniforms[SpotLightIndex].Direction,
											SpotLightsUniforms[SpotLightIndex].CutOffAngleCos,
											SpotLightsUniforms[SpotLightIndex].Exponent,
											SpotLightsUniforms[SpotLightIndex].Linear,
											SpotLightsUniforms[SpotLightIndex].Constant,
											SpotLightsUniforms[SpotLightIndex].InnerRadius,
											SpotLightsUniforms[SpotLightIndex].OuterRadius,
											SpotLightsUniforms[SpotLightIndex].RadiusSharpness);
	}
}

void Shader::SetTextureUnit(GLint TextureUnit) const
{
	glUniform1i(UniformTexture, TextureUnit);
}

void Shader::SetDirectionalShadowMap(GLint TextureUnit) const
{
	glUniform1i(UniformDirectionalShadowMap, TextureUnit);
}

void Shader::SetDirectionalLightSpaceTransform(const glm::mat4& Transform) const
{
	glUniformMatrix4fv(UniformDirectionalLightSpaceTransform, 
						1, GL_FALSE, 
						glm::value_ptr(Transform));
}

void Shader::SetLightMatrices(const std::array<glm::mat4, 6>& LightMatrices) const
{
	for (int Face = 0; Face < 6; ++Face)
	{
		glUniformMatrix4fv(UniformLightMatrices[Face],
							1,
							GL_FALSE,
							glm::value_ptr(LightMatrices.at(Face)));
	}
}

void Shader::Use() const
{
	glUseProgram(Id);
}

void Shader::Clear()
{
	if (Id != 0)
	{
		glDeleteProgram(Id);
		Id = 0;
	}
}

GLuint Shader::GetId() const
{
	return Id;
}

void Shader::CompileProgram()
{
	GLint Result = 0;
	GLchar ELog[1024] = {0};

	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(Id, sizeof(ELog), nullptr, ELog);
		printf("Error linking program: '%s'\n", ELog);
		return;
	}

	glValidateProgram(Id);
	glGetProgramiv(Id, GL_VALIDATE_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(Id, sizeof(ELog), nullptr, ELog);
		printf("Error validating program: '%s'\n", ELog);
		return;
	}

	// Setup uniform variables
	UniformProjection = glGetUniformLocation(Id, "ProjectionMatrix");
	UniformView = glGetUniformLocation(Id, "ViewMatrix");
	UniformModel = glGetUniformLocation(Id, "ModelMatrix");

	DirectionalLightUniforms.Color = glGetUniformLocation(Id, "SunLight.Base.Color");
	DirectionalLightUniforms.AmbientIntensity = glGetUniformLocation(Id, "SunLight.Base.AmbientIntensity");
	DirectionalLightUniforms.DiffuseIntensity = glGetUniformLocation(Id, "SunLight.Base.DiffuseIntensity");
	DirectionalLightUniforms.Direction = glGetUniformLocation(Id, "SunLight.Direction");

	UniformPointLightsCount = glGetUniformLocation(Id, "PointLightsCount");

	for (int PointLightIndex = 0; PointLightIndex < Helper::MAX_POINT_LIGHTS; PointLightIndex++)
	{
		char LocationsBuffer[100] = { '\0' };

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Base.Color", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Color = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Base.AmbientIntensity", PointLightIndex);
		PointLightsUniforms[PointLightIndex].AmbientIntensity = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Base.DiffuseIntensity", PointLightIndex);
		PointLightsUniforms[PointLightIndex].DiffuseIntensity = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Position", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Position = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Exponent", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Exponent = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Linear", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Linear = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].Constant", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Constant = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].InnerRadius", PointLightIndex);
		PointLightsUniforms[PointLightIndex].InnerRadius = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].OuterRadius", PointLightIndex);
		PointLightsUniforms[PointLightIndex].OuterRadius = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "PointLights[%d].RadiusSharpness", PointLightIndex);
		PointLightsUniforms[PointLightIndex].RadiusSharpness = glGetUniformLocation(Id, LocationsBuffer);
	}

	UniformSpotLightsCount = glGetUniformLocation(Id, "SpotLightsCount");

	for (int SpotLightIndex = 0; SpotLightIndex < Helper::MAX_SPOT_LIGHTS; SpotLightIndex++)
	{
		char LocationsBuffer[100] = { '\0' };

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Base.Color", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Color = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Base.AmbientIntensity", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].AmbientIntensity = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Base.DiffuseIntensity", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].DiffuseIntensity = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Position", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Position = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].Direction", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Direction = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].CutOffAngleCos", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].CutOffAngleCos = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Exponent", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Exponent = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Linear", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Linear = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.Constant", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Constant = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.InnerRadius", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].InnerRadius = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.OuterRadius", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].OuterRadius = glGetUniformLocation(Id, LocationsBuffer);

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "SpotLights[%d].PointLight.RadiusSharpness", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].RadiusSharpness = glGetUniformLocation(Id, LocationsBuffer);
	}

	UniformCameraPosition = glGetUniformLocation(Id, "CameraPosition");

	UniformMaterialShininess = glGetUniformLocation(Id, "Material.Shininess");
	UniformMaterialSpecularIntensity = glGetUniformLocation(Id, "Material.SpecularIntensity");

	UniformTexture = glGetUniformLocation(Id, "Texture");
	UniformDirectionalShadowMap = glGetUniformLocation(Id, "DirectionalShadowMap");
	UniformDirectionalLightSpaceTransform = glGetUniformLocation(Id, "DirectionalLightSpaceTransform");

	UniformOmniLightPosition = glGetUniformLocation(Id, "LightPosition");
	UniformFarPlane = glGetUniformLocation(Id, "FarPlane");

	for (int Face = 0; Face < 6; ++Face)
	{
		char LocationsBuffer[100] = { '\0' };

		snprintf(LocationsBuffer, sizeof(LocationsBuffer), "LightMatrices[%d]", Face);
		UniformLightMatrices[Face] = glGetUniformLocation(Id, LocationsBuffer);
	}
}

void Shader::CompileShaders(const char* VertexShaderCode,
							const char* FragmentShaderCode)
{
	Id = glCreateProgram();

	if (!Id)
	{
		printf("Error creating shader program %s %i", __FILE__, __LINE__);
		return;
	}

	AddShader(Id, VertexShaderCode, GL_VERTEX_SHADER);
	AddShader(Id, FragmentShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShaders(const char* VertexShaderCode,
							const char* GeometryShaderCode,
							const char* FragmentShaderCode)
{
	Id = glCreateProgram();

	if (!Id)
	{
		printf("Error creating shader program %s %i", __FILE__, __LINE__);
		return;
	}

	AddShader(Id, VertexShaderCode, GL_VERTEX_SHADER);
	AddShader(Id, GeometryShaderCode, GL_GEOMETRY_SHADER);
	AddShader(Id, FragmentShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::AddShader(GLuint ShaderProgramId, const char* ShaderCode, GLenum ShaderType) const
{
	// Create shader
	const GLuint LocalShader = glCreateShader(ShaderType);

	// Type conversion to GL types
	const GLchar* LocalShaderCode[1];
	LocalShaderCode[0] = ShaderCode;

	GLint CodeLength [1];
	CodeLength[0] = strlen(ShaderCode);

	// Set shader source and compile shader
	glShaderSource(LocalShader, 1, LocalShaderCode, CodeLength);
	glCompileShader(LocalShader);

	// Check shader compilation status
	GLint Result = 0;
	GLchar ELog[1024] = {0};

	glGetShaderiv(LocalShader, GL_COMPILE_STATUS, &Result);

	if (!Result)
	{
		glGetShaderInfoLog(LocalShader, sizeof(ELog), nullptr, ELog);
		printf("Error compiling the %d shader: '%s'\n", ShaderType, ELog);
		return;
	}

	// Attach compiled shader to the shader program
	glAttachShader(ShaderProgramId, LocalShader);
}

std::string Shader::ReadFile(const char* FilePath)
{
	std::ifstream FileStream(FilePath, std::ios::in);

	if (!FileStream.is_open())
	{
		std::cerr << "Error: Unable to open file: " << FilePath << std::endl;
		return "";
	}

	std::stringstream Buffer;
	Buffer << FileStream.rdbuf();

	std::string Content = Buffer.str();

	FileStream.close();

	//std::cout << "File Content (" << FilePath << "):\n\n" << Content << std::endl;

	return Content;
}
