#include "Core/Level.h"

#include <geometric.hpp>

#include "Core/Actor.h"
#include "Render/PointLight.h"
#include "Render/SpotLight.h"
#include "Render/DirectionalLight.h"

Level::Level()
{
	Initialize();
}

void Level::Initialize()
{
	SunLight = std::make_shared<DirectionalLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
													0.3f, 
													0.8f,
													glm::normalize(glm::vec3(-2.0f, -2.0f, 0.3f)),
													4096, 4096);

	// TODO Move to GameInstance
	CreateSkybox();

	EditorGrid = std::make_unique<Grid>(100, 50);

	AddPointLight(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
					0.0f, 5.0f,
					glm::vec3(0.0f, 0.8f, 0.0f),
					0.3f, 0.2f, 0.1f,
					3.0f, 15.0f, 10.0f);

	AddPointLight(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
					0.0f, 5.0f,
					glm::vec3(10.0f, 0.8f, 0.0f),
					0.3f, 0.2f, 0.1f,
					5.0f, 15.0f, 15.0f);


	AddPointLight(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
					0.0f, 5.0f,
					glm::vec3(5.0f, 0.8f, 0.0f),
					0.3f, 0.2f, 0.1f,
					6.0f, 15.0f, 15.0f);

	AddSpotLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f, 2.0f,
					glm::vec3(0.0f, 0.8f, 3.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					20.0f,
					0.3f, 0.2f, 0.1f,
					4.0f, 10.0f, 10.0f);
}

void Level::AddActor(const std::shared_ptr<Actor>& InActor)
{
	std::string Name = InActor->GetName();

	if (ActorMap.contains(Name))
	{
		Name += "_" + std::to_string(ActorMap.size());
		InActor->SetName(Name);
	}

	Actors.push_back(InActor);
	ActorMap[Name] = InActor;
}

std::shared_ptr<Actor> Level::GetActor(const std::string& Name) const
{
	const auto ActorIt = ActorMap.find(Name);
	return (ActorIt != ActorMap.end()) ? ActorIt->second : nullptr;
}

const std::vector<std::shared_ptr<Actor>>& Level::GetAllActors() const
{
	return Actors;
}

void Level::Update(float DeltaTime)
{
	for (const auto& Actor : Actors)
	{
		Actor->Update(DeltaTime);
	}
}

void Level::AddPointLight(glm::vec4 InitColor, GLfloat InitAmbientIntensity, GLfloat InitDiffuseIntensity,
	glm::vec3 InitPosition, GLfloat InitExponent, GLfloat InitLinear, GLfloat InitConstant, GLfloat InitInnerRadius,
	GLfloat InitOuterRadius, GLfloat InitRadiusSharpness, GLint InitShadowWidth, GLint InitShadowHeight,
	GLfloat InitNear, GLfloat InitFar)
{
	PointLights.emplace_back(std::make_shared<PointLight>(
		InitColor, 
		InitAmbientIntensity, 
		InitDiffuseIntensity, 
		InitPosition, 
		InitExponent, 
		InitLinear, 
		InitConstant, 
		InitInnerRadius, 
		InitOuterRadius, 
		InitRadiusSharpness, 
		InitShadowWidth, 
		InitShadowHeight, 
		InitNear, 
		InitFar
	));
}

void Level::AddSpotLight(glm::vec4 InitColor, GLfloat InitAmbientIntensity, GLfloat InitDiffuseIntensity,
	glm::vec3 InitPosition, glm::vec3 InitDirection, GLfloat InitCutOffAngle, GLfloat InitExponent, GLfloat InitLinear,
	GLfloat InitConstant, GLfloat InitInnerRadius, GLfloat InitOuterRadius, GLfloat InitRadiusSharpness,
	GLint InitShadowWidth, GLint InitShadowHeight, GLfloat InitNear, GLfloat InitFar)
{
	SpotLights.emplace_back(std::make_shared<SpotLight>(
			InitColor, 
			InitAmbientIntensity, 
			InitDiffuseIntensity, 
			InitPosition, 
			InitDirection, 
			InitCutOffAngle, 
			InitExponent, 
			InitLinear, 
			InitConstant, 
			InitInnerRadius, 
			InitOuterRadius, 
			InitRadiusSharpness, 
			InitShadowWidth, 
			InitShadowHeight, 
			InitNear, 
			InitFar
	));
}

std::shared_ptr<DirectionalLight>& Level::GetSunLight()
{
	return SunLight;
}

std::vector<std::shared_ptr<PointLight>>& Level::GetPointLights()
{
	return PointLights;
}

std::vector<std::shared_ptr<SpotLight>>& Level::GetSpotLights()
{
	return SpotLights;
}

void Level::CreateSkybox()
{
	std::array<std::string, 6> FaceLocations;

	FaceLocations.at(0) = "Content/Textures/Skybox/sky_36/pz.png";
	FaceLocations.at(1) = "Content/Textures/Skybox/sky_36/nz.png";
	FaceLocations.at(2) = "Content/Textures/Skybox/sky_36/py.png";
	FaceLocations.at(3) = "Content/Textures/Skybox/sky_36/ny.png";
	FaceLocations.at(4) = "Content/Textures/Skybox/sky_36/nx.png";
	FaceLocations.at(5) = "Content/Textures/Skybox/sky_36/px.png";

	Sky = std::make_unique<Skybox>(FaceLocations);
}

std::unique_ptr<Skybox>& Level::GetSkybox()
{
	return Sky;
}

std::unique_ptr<Grid>& Level::GetEditorGrid()
{
	return EditorGrid;
}
