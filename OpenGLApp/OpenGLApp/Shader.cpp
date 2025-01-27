#include "Shader.h"

#include <iostream>
#include <format> 
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

void Shader::SetPointLights(const std::vector<PointLight>& PointLights,
							unsigned int StartUnit,
							unsigned int Offset) const
{
	const int PointLightsCount = PointLights.size() > Helper::MAX_POINT_LIGHTS
											? Helper::MAX_POINT_LIGHTS
											: PointLights.size();

	glUniform1i(UniformPointLightsCount, PointLightsCount);

	for (int PointLightIndex = 0; PointLightIndex < PointLightsCount; PointLightIndex++)
	{
		PointLights.at(PointLightIndex).Apply(PointLightsUniforms[PointLightIndex].Color,
											PointLightsUniforms[PointLightIndex].AmbientIntensity,
											PointLightsUniforms[PointLightIndex].DiffuseIntensity,
											PointLightsUniforms[PointLightIndex].Position,
											PointLightsUniforms[PointLightIndex].Exponent,
											PointLightsUniforms[PointLightIndex].Linear,
											PointLightsUniforms[PointLightIndex].Constant,
											PointLightsUniforms[PointLightIndex].InnerRadius,
											PointLightsUniforms[PointLightIndex].OuterRadius,
											PointLightsUniforms[PointLightIndex].RadiusSharpness);

		PointLights.at(PointLightIndex).GetShadowMap()->
								Read(GL_TEXTURE0 + StartUnit + PointLightIndex);

		glUniform1i(OmniShadowMapsUniforms.at(PointLightIndex + Offset).ShadowMap, PointLightIndex + StartUnit);
		glUniform1f(OmniShadowMapsUniforms.at(PointLightIndex + Offset).FarPlane, PointLights.at(PointLightIndex).GetFarPlane());
	}
}

void Shader::SetSpotLights(const std::vector<SpotLight>& SpotLights,
							unsigned int StartUnit,
							unsigned int Offset) const
{
	const int SpotLightsCount = SpotLights.size() > Helper::MAX_SPOT_LIGHTS
											? Helper::MAX_SPOT_LIGHTS
											: SpotLights.size();

	glUniform1i(UniformSpotLightsCount, SpotLightsCount);

	for (int SpotLightIndex = 0; SpotLightIndex < SpotLightsCount; SpotLightIndex++)
	{
		SpotLights.at(SpotLightIndex).Apply(SpotLightsUniforms[SpotLightIndex].Color,
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

		SpotLights.at(SpotLightIndex).GetShadowMap()->
								Read(GL_TEXTURE0 + StartUnit + SpotLightIndex);

		glUniform1i(OmniShadowMapsUniforms.at(SpotLightIndex + Offset).ShadowMap, SpotLightIndex + StartUnit);
		glUniform1f(OmniShadowMapsUniforms.at(SpotLightIndex + Offset).FarPlane, SpotLights.at(SpotLightIndex).GetFarPlane());
	}
}

void Shader::SetTextureUnit(GLint TextureUnit) const
{
	glUniform1i(UniformTexture, TextureUnit);
}

void Shader::SetSkyboxTextureUnit(GLint TextureUnit) const
{
	glUniform1i(UniformSkybox, TextureUnit);
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

void Shader::Validate() const
{
	GLint Result = 0;
	GLchar ELog[1024] = {0};

	glValidateProgram(Id);
	glGetProgramiv(Id, GL_VALIDATE_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(Id, sizeof(ELog), nullptr, ELog);
		printf("Error validating program: '%s'\n", ELog);
		return;
	}
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
		std::string Location;

		Location = std::format("PointLights[{}].Base.Color", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Color = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Base.AmbientIntensity", PointLightIndex);
		PointLightsUniforms[PointLightIndex].AmbientIntensity = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Base.DiffuseIntensity", PointLightIndex);
		PointLightsUniforms[PointLightIndex].DiffuseIntensity = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Position", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Position = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Exponent", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Exponent = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Linear", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Linear = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].Constant", PointLightIndex);
		PointLightsUniforms[PointLightIndex].Constant = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].InnerRadius", PointLightIndex);
		PointLightsUniforms[PointLightIndex].InnerRadius = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].OuterRadius", PointLightIndex);
		PointLightsUniforms[PointLightIndex].OuterRadius = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("PointLights[{}].RadiusSharpness", PointLightIndex);
		PointLightsUniforms[PointLightIndex].RadiusSharpness = glGetUniformLocation(Id, Location.c_str());
	}

	UniformSpotLightsCount = glGetUniformLocation(Id, "SpotLightsCount");

	for (int SpotLightIndex = 0; SpotLightIndex < Helper::MAX_SPOT_LIGHTS; SpotLightIndex++)
	{
		std::string Location;

		Location = std::format("SpotLights[{}].PointLight.Base.Color", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Color = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Base.AmbientIntensity", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].AmbientIntensity = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Base.DiffuseIntensity", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].DiffuseIntensity = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Position", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Position = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].Direction", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Direction = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].CutOffAngleCos", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].CutOffAngleCos = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Exponent", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Exponent = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Linear", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Linear = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.Constant", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].Constant = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.InnerRadius", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].InnerRadius = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.OuterRadius", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].OuterRadius = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("SpotLights[{}].PointLight.RadiusSharpness", SpotLightIndex);
		SpotLightsUniforms[SpotLightIndex].RadiusSharpness = glGetUniformLocation(Id, Location.c_str());
	}

	UniformCameraPosition = glGetUniformLocation(Id, "CameraPosition");

	UniformMaterialShininess = glGetUniformLocation(Id, "Material.Shininess");
	UniformMaterialSpecularIntensity = glGetUniformLocation(Id, "Material.SpecularIntensity");

	UniformTexture = glGetUniformLocation(Id, "Texture");
	UniformDirectionalShadowMap = glGetUniformLocation(Id, "DirectionalShadowMap");
	UniformDirectionalLightSpaceTransform = glGetUniformLocation(Id, "DirectionalLightSpaceTransform");

	UniformOmniLightPosition = glGetUniformLocation(Id, "OmniLightPosition");
	UniformFarPlane = glGetUniformLocation(Id, "FarPlane");

	for (int Index = 0; Index < 6; ++Index)
	{
		std::string Location = std::format("LightMatrices[{}]", Index);
		UniformLightMatrices[Index] = glGetUniformLocation(Id, Location.c_str());
	}

	for (int ShadowIndex = 0; ShadowIndex < Helper::MAX_POINT_LIGHTS + Helper::MAX_SPOT_LIGHTS; ++ShadowIndex)
	{
		std::string Location;

		Location = std::format("OmniShadowMaps[{}].ShadowMap", ShadowIndex);
		OmniShadowMapsUniforms[ShadowIndex].ShadowMap = glGetUniformLocation(Id, Location.c_str());

		Location = std::format("OmniShadowMaps[{}].FarPlane", ShadowIndex);
		OmniShadowMapsUniforms[ShadowIndex].FarPlane = glGetUniformLocation(Id, Location.c_str());
	}

	UniformSkybox = glGetUniformLocation(Id, "Skybox");
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
