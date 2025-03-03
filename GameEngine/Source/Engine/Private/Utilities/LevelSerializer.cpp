#include "LevelSerializer.h"
#include <fstream>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "Core/Level.h"
#include "Core/Actor.h"
#include "Render/PointLight.h"
#include "Render/SpotLight.h"
#include "Render/DirectionalLight.h"
#include "Components/Grid.h"
#include "Components/Skybox.h"

void LevelSerializer::SaveLevel(const std::string& FilePath, const Level& CurrentLevel)
{
	std::ofstream OutStream(FilePath, std::ios::binary);

	if (!OutStream)
	{
		std::cerr << "Failed to open file for saving level." << std::endl;
		return;
	}

	// Save actors
	uint32_t ActorCount = CurrentLevel.GetAllActors().size();
	ActorCount = ToLittleEndian(ActorCount); // Convert to Little Endian before saving
	OutStream.write(reinterpret_cast<const char*>(&ActorCount), sizeof(uint32_t));

	for (const auto& Actor : CurrentLevel.GetAllActors())
	{
		SaveActor(OutStream, Actor);
	}

	// Save point lights
	uint32_t PointLightCount = CurrentLevel.GetPointLights().size();
	PointLightCount = ToLittleEndian(PointLightCount); // Convert to Little Endian before saving
	OutStream.write(reinterpret_cast<const char*>(&PointLightCount), sizeof(uint32_t));

	for (const auto& PointLight : CurrentLevel.GetPointLights())
	{
		SavePointLight(OutStream, PointLight);
	}

	// Save spot lights
	uint32_t SpotLightCount = CurrentLevel.GetSpotLights().size();
	SpotLightCount = ToLittleEndian(SpotLightCount); // Convert to Little Endian before saving
	OutStream.write(reinterpret_cast<const char*>(&SpotLightCount), sizeof(uint32_t));

	for (const auto& SpotLight : CurrentLevel.GetSpotLights())
	{
		SaveSpotLight(OutStream, SpotLight);
	}

	// Save directional light
	SaveDirectionalLight(OutStream, CurrentLevel.GetSunLight());

	// Save grid
	SaveGrid(OutStream, CurrentLevel.GetEditorGrid());

	// Save skybox
	SaveSkybox(OutStream, CurrentLevel);
}

std::shared_ptr<Level> LevelSerializer::LoadLevel(const std::string& FilePath)
{
	std::ifstream InStream(FilePath, std::ios::binary);
	if (!InStream)
	{
		std::cerr << "Failed to open file for loading level." << std::endl;
		return nullptr;
	}

	// Create a new Level instance
	std::shared_ptr<Level> CurrentLevel = std::make_shared<Level>();

	// Load actors
	uint32_t ActorCount;
	InStream.read(reinterpret_cast<char*>(&ActorCount), sizeof(uint32_t));
	ActorCount = FromLittleEndian(ActorCount);  // Convert from Little Endian to native endian

	for (uint32_t i = 0; i < ActorCount; i++)
	{
		LoadActor(InStream, *CurrentLevel);
	}

	// Load point lights
	uint32_t PointLightCount;
	InStream.read(reinterpret_cast<char*>(&PointLightCount), sizeof(uint32_t));
	PointLightCount = FromLittleEndian(PointLightCount);  // Convert from Little Endian to native endian

	for (uint32_t i = 0; i < PointLightCount; i++)
	{
		LoadPointLight(InStream, *CurrentLevel);
	}

	// Load spot lights
	uint32_t SpotLightCount;
	InStream.read(reinterpret_cast<char*>(&SpotLightCount), sizeof(uint32_t));
	SpotLightCount = FromLittleEndian(SpotLightCount);  // Convert from Little Endian to native endian

	for (uint32_t i = 0; i < SpotLightCount; i++)
	{
		LoadSpotLight(InStream, *CurrentLevel);
	}

	// Load directional light
	LoadDirectionalLight(InStream, CurrentLevel);

	// Load grid
	LoadGrid(InStream, *CurrentLevel);

	// Load skybox
	LoadSkybox(InStream, *CurrentLevel);

	return CurrentLevel;
}

void LevelSerializer::SaveActor(std::ofstream& OutStream, const std::shared_ptr<Actor>& InActor)
{
	//// Save actor name
	//uint32_t nameLength = InActor->GetName().size();
	//nameLength = ToLittleEndian(nameLength);  // Convert to Little Endian before saving
	//OutStream.write(reinterpret_cast<const char*>(&nameLength), sizeof(uint32_t));
	//OutStream.write(InActor->GetName().c_str(), nameLength);

	//// Save actor transform
	//OutStream.write(reinterpret_cast<const char*>(&InActor->GetTransform().Location), sizeof(glm::vec3));
	//OutStream.write(reinterpret_cast<const char*>(&InActor->GetTransform().Rotation), sizeof(glm::quat));
	//OutStream.write(reinterpret_cast<const char*>(&InActor->GetTransform().Scale), sizeof(glm::vec3));

	//// Save actor type (this can be expanded to handle different actor types)
	//uint32_t actorType = InActor->GetActorType();
	//actorType = ToLittleEndian(actorType);  // Convert to Little Endian before saving
	//OutStream.write(reinterpret_cast<const char*>(&actorType), sizeof(uint32_t));
}

void LevelSerializer::LoadActor(std::ifstream& inStream, Level& level)
{
	//// Load actor name
	//uint32_t nameLength;
	//inStream.read(reinterpret_cast<char*>(&nameLength), sizeof(uint32_t));
	//nameLength = FromLittleEndian(nameLength);  // Convert from Little Endian to native endian
	//std::string actorName(nameLength, '\0');
	//inStream.read(&actorName[0], nameLength);

	//// Load actor transform
	//glm::vec3 location, scale;
	//glm::quat rotation;
	//inStream.read(reinterpret_cast<char*>(&location), sizeof(glm::vec3));
	//inStream.read(reinterpret_cast<char*>(&rotation), sizeof(glm::quat));
	//inStream.read(reinterpret_cast<char*>(&scale), sizeof(glm::vec3));

	//// Create new actor and add to level
	//std::shared_ptr<Actor> actor = std::make_shared<Actor>(actorName, location, rotation, scale);
	//level.AddActor(actor);
}

void LevelSerializer::SavePointLight(std::ofstream& OutStream, const std::shared_ptr<PointLight>& PointLight)
{
	const glm::vec4 Color = PointLight->GetColor();
	const glm::vec3 Position = PointLight->GetPosition();
	const GLfloat AmbientIntensity = PointLight->GetAmbientIntensity();
	const GLfloat DiffuseIntensity = PointLight->GetDiffuseIntensity();
	const GLfloat Exponent = PointLight->GetExponent();
	const GLfloat Linear = PointLight->GetLinear();
	const GLfloat Constant = PointLight->GetConstant();
	const GLfloat InnerRadius = PointLight->GetInnerRadius();
	const GLfloat OuterRadius = PointLight->GetOuterRadius();
	const GLfloat RadiusSharpness = PointLight->GetRadiusSharpness();
	const GLfloat Near = PointLight->GetNear();
	const GLfloat Far = PointLight->GetFarPlane();

	OutStream.write(reinterpret_cast<const char*>(&Color), sizeof(glm::vec4));
	OutStream.write(reinterpret_cast<const char*>(&Position), sizeof(glm::vec3));
	OutStream.write(reinterpret_cast<const char*>(&AmbientIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&DiffuseIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Exponent), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Linear), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Constant), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&InnerRadius), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&OuterRadius), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&RadiusSharpness), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Near), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Far), sizeof(GLfloat));
}

void LevelSerializer::LoadPointLight(std::ifstream& InStream, Level& Level)
{
	glm::vec4 Color;
	glm::vec3 Position;
	GLfloat AmbientIntensity, DiffuseIntensity;
	GLfloat Exponent, Linear, Constant;
	GLfloat InnerRadius, OuterRadius, RadiusSharpness;
	GLfloat Near, Far;

	InStream.read(reinterpret_cast<char*>(&Color), sizeof(glm::vec4));
	InStream.read(reinterpret_cast<char*>(&Position), sizeof(glm::vec3));
	InStream.read(reinterpret_cast<char*>(&AmbientIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&DiffuseIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Exponent), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Linear), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Constant), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&InnerRadius), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&OuterRadius), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&RadiusSharpness), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Near), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Far), sizeof(GLfloat));

	Level.AddPointLight(std::make_shared<PointLight>(Color,
													AmbientIntensity,
													DiffuseIntensity,
													Position,
													Exponent,
													Linear,
													Constant,
													InnerRadius,
													OuterRadius,
													RadiusSharpness,
													1024,
													1024,
													Near,
													Far));
}

void LevelSerializer::SaveSpotLight(std::ofstream& OutStream, const std::shared_ptr<SpotLight>& InSpotLight)
{
	const glm::vec4 Color = InSpotLight->GetColor();
	const GLfloat AmbientIntensity = InSpotLight->GetAmbientIntensity();
	const GLfloat DiffuseIntensity = InSpotLight->GetDiffuseIntensity();
	const glm::vec3 Position = InSpotLight->GetPosition();
	const glm::vec3 Direction = InSpotLight->GetDirection();
	const GLfloat CutOffAngleCos = InSpotLight->GetCutOffAngleCos();
	const GLfloat Exponent = InSpotLight->GetExponent();
	const GLfloat Linear = InSpotLight->GetLinear();
	const GLfloat Constant = InSpotLight->GetConstant();
	const GLfloat InnerRadius = InSpotLight->GetInnerRadius();
	const GLfloat OuterRadius = InSpotLight->GetOuterRadius();
	const GLfloat RadiusSharpness = InSpotLight->GetRadiusSharpness();
	const GLfloat Near = InSpotLight->GetNear();
	const GLfloat Far = InSpotLight->GetFarPlane();

	OutStream.write(reinterpret_cast<const char*>(&Color), sizeof(glm::vec4));
	OutStream.write(reinterpret_cast<const char*>(&AmbientIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&DiffuseIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Position), sizeof(glm::vec3));
	OutStream.write(reinterpret_cast<const char*>(&Direction), sizeof(glm::vec3));
	OutStream.write(reinterpret_cast<const char*>(&CutOffAngleCos), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Exponent), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Linear), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Constant), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&InnerRadius), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&OuterRadius), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&RadiusSharpness), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Near), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Far), sizeof(GLfloat));
}

void LevelSerializer::LoadSpotLight(std::ifstream& InStream, Level& CurrentLevel)
{
	glm::vec4 Color;
	GLfloat AmbientIntensity, DiffuseIntensity;
	glm::vec3 Position, Direction;
	GLfloat CutOffAngle, Exponent, Linear, Constant;
	GLfloat InnerRadius, OuterRadius, RadiusSharpness;
	GLfloat Near, Far;

	InStream.read(reinterpret_cast<char*>(&Color), sizeof(glm::vec4));
	InStream.read(reinterpret_cast<char*>(&AmbientIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&DiffuseIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Position), sizeof(glm::vec3));
	InStream.read(reinterpret_cast<char*>(&Direction), sizeof(glm::vec3));
	InStream.read(reinterpret_cast<char*>(&CutOffAngle), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Exponent), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Linear), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Constant), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&InnerRadius), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&OuterRadius), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&RadiusSharpness), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Near), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Far), sizeof(GLfloat));

	CurrentLevel.AddSpotLight(std::make_shared<SpotLight>(Color,
															AmbientIntensity,
															DiffuseIntensity,
															Position,
															Direction,
															CutOffAngle,
															Exponent,
															Linear,
															Constant,
															InnerRadius,
															OuterRadius,
															RadiusSharpness,
															1024,
															1024,
															Near,
															Far
															));
}

void LevelSerializer::SaveDirectionalLight(std::ofstream& OutStream, const std::shared_ptr<DirectionalLight>& InDirectionalLight)
{
	const glm::vec4 Color = InDirectionalLight->GetColor();
	const GLfloat AmbientIntensity = InDirectionalLight->GetAmbientIntensity();
	const GLfloat DiffuseIntensity = InDirectionalLight->GetDiffuseIntensity();
	const glm::vec3 Direction = InDirectionalLight->GetDirection();
	constexpr GLint ShadowWidth = 4096;
	constexpr GLint ShadowHeight = 4096;

	OutStream.write(reinterpret_cast<const char*>(&Color), sizeof(glm::vec4));
	OutStream.write(reinterpret_cast<const char*>(&AmbientIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&DiffuseIntensity), sizeof(GLfloat));
	OutStream.write(reinterpret_cast<const char*>(&Direction), sizeof(glm::vec3));
	OutStream.write(reinterpret_cast<const char*>(&ShadowWidth), sizeof(GLint));
	OutStream.write(reinterpret_cast<const char*>(&ShadowHeight), sizeof(GLint));
}

void LevelSerializer::LoadDirectionalLight(std::ifstream& InStream, std::shared_ptr<Level>& CurrentLevel)
{
	glm::vec4 Color;
	GLfloat AmbientIntensity, DiffuseIntensity;
	glm::vec3 Direction;
	GLint ShadowWidth, ShadowHeight;

	InStream.read(reinterpret_cast<char*>(&Color), sizeof(glm::vec4));
	InStream.read(reinterpret_cast<char*>(&AmbientIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&DiffuseIntensity), sizeof(GLfloat));
	InStream.read(reinterpret_cast<char*>(&Direction), sizeof(glm::vec3));
	InStream.read(reinterpret_cast<char*>(&ShadowWidth), sizeof(GLint));
	InStream.read(reinterpret_cast<char*>(&ShadowHeight), sizeof(GLint));

	CurrentLevel->SetSunLight(std::make_shared<DirectionalLight>(Color,
																AmbientIntensity,
																DiffuseIntensity,
																Direction,
																ShadowWidth,
																ShadowHeight
																));
}

void LevelSerializer::SaveGrid(std::ofstream& OutStream, const std::unique_ptr<Grid>& InGrid)
{
	const float Size = InGrid->GetSize();
	const int Divisions = InGrid->GetDivisions();

	OutStream.write(reinterpret_cast<const char*>(&Size), sizeof(float));
	OutStream.write(reinterpret_cast<const char*>(&Divisions), sizeof(int));
}

void LevelSerializer::LoadGrid(std::ifstream& InStream, Level& CurrentLevel)
{
	float Size;
	int Divisions;

	InStream.read(reinterpret_cast<char*>(&Size), sizeof(float));
	InStream.read(reinterpret_cast<char*>(&Divisions), sizeof(int));

	Size = FromLittleEndian(Size);
	Divisions = FromLittleEndian(Divisions);

	CurrentLevel.SetGrid(std::make_unique<Grid>(Size, Divisions));
}

void LevelSerializer::SaveSkybox(std::ofstream& OutStream, const Level& CurrentLevel)
{
	std::string SkyboxPath = CurrentLevel.GetSkyboxPath();

	uint32_t PathLength = static_cast<uint32_t>(SkyboxPath.size());
	PathLength = ToLittleEndian(PathLength);
	OutStream.write(reinterpret_cast<const char*>(&PathLength), sizeof(PathLength));

	OutStream.write(SkyboxPath.c_str(), PathLength);
}

void LevelSerializer::LoadSkybox(std::ifstream& InStream, Level& CurrentLevel)
{
	uint32_t PathLength;
	InStream.read(reinterpret_cast<char*>(&PathLength), sizeof(PathLength));
	PathLength = FromLittleEndian(PathLength);

	std::string SkyboxPath(PathLength, '\0');
	InStream.read(SkyboxPath.data(), PathLength);

	CurrentLevel.SetSkybox(std::move(SkyboxPath));
}

uint32_t LevelSerializer::ToLittleEndian(uint32_t value)
{
	return ((value >> 24) & 0x000000FF) |
			((value << 8)  & 0x00FF0000) |
			((value >> 8)  & 0x0000FF00) |
			((value << 24) & 0xFF000000);
}

uint32_t LevelSerializer::FromLittleEndian(uint32_t value)
{
	return ((value >> 24) & 0x000000FF) |
			((value << 8)  & 0x00FF0000) |
			((value >> 8)  & 0x0000FF00) |
			((value << 24) & 0xFF000000);
}