#include "Core/Level.h"

#include <ranges>
#include <geometric.hpp>

#include "Core/Actor.h"
#include "Render/PointLight.h"
#include "Render/SpotLight.h"
#include "Render/DirectionalLight.h"

#include "Components/Mesh.h"
#include "Components/Material.h"
#include "Components/SkeletalMeshComponent.h"

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
	LoadTextures();
	LoadMaterials();

	CreateSkybox();
	Create3DObjects();

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

	for (const auto& ActorIt : Actors)
	{
		ActorIt->Initialize();
	}
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

std::vector<std::shared_ptr<Mesh>>& Level::GetMeshList()
{
	return MeshList;
}

std::map<const char*, std::shared_ptr<Texture>>& Level::GetTextures()
{
	return Textures;
}

std::map<const char*, std::shared_ptr<Material>>& Level::GetMaterials()
{
	return Materials;
}

void Level::LoadTextures()
{
	PlaceholderTexture->LoadTexture_RGBA();

	Textures.emplace("Brick", std::make_shared<Texture>("Content/Textures/brick.jpg"));
	Textures.emplace("Rock", std::make_shared<Texture>("Content/Textures/rock.jpg"));
	Textures.emplace("Metal", std::make_shared<Texture>("Content/Textures/metal.jpg"));
	Textures.emplace("Sand", std::make_shared<Texture>("Content/Textures/sand.png"));
	Textures.emplace("Gold", std::make_shared<Texture>("Content/Textures/gold.jpg"));
	Textures.emplace("Grid", std::make_shared<Texture>("Content/Textures/small_grid.png"));
	Textures.emplace("Plain", std::make_shared<Texture>("Content/Textures/plain.png"));

	for (const auto& Texture : Textures | std::views::values)
	{
		Texture->LoadTexture_RGBA();
	}
}

void Level::LoadMaterials()
{
	Materials.emplace("Metal", std::make_shared<Material>(5.0f, 128.0f));
	Materials.emplace("Mat", std::make_shared<Material>(0.5f, 1.0f));
}

void Level::Create3DObjects()
{
	// Define floor vertices
	std::vector<GLfloat> FloorVertices = {
	//	   x      y      z      u      v     nx     ny    nz
		-10.0f, 0.0f, -10.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 10.0f,  0.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,  0.0f, 10.0f, 0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f, 10.0f, 10.0f, 0.0f, 1.0f, 0.0f
	};

	std::vector<unsigned int> FloorIndices = {
		0, 2, 1,
		1, 2, 3
	};

	//TODO move to mesh with calculate normals method or use compute shader
	// Represents faces created from further represented vertices
	std::vector<unsigned int> Indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Define triangle's vertices
	std::vector<GLfloat> Vertices = {
	//	  x      y      z     u     v     nx    ny    nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  1.0,   0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	Helper::CalculateAverageNormals(Indices, Vertices, 8, 5);

	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_shared<Mesh>(FloorVertices, FloorIndices));

	const auto PonyActor = std::make_shared<Actor>("PonyCar");
	PonyActor->SetRootComponent(std::make_shared<SkeletalMeshComponent>("PonyMesh", "Content/Meshes/Pony_cartoon.obj"));

	PonyActor->GetRootComponent()->SetLocation(glm::vec3(2.0f, -1.0f, -2.0f));
	PonyActor->GetRootComponent()->SetRotation(0.0f, 30.0f, 0.0f);
	PonyActor->GetRootComponent()->SetScale(0.07f, 0.07f, 0.07f);

	AddActor(PonyActor);

	std::vector<std::shared_ptr<Texture>> TextureVec;
	for (auto& Tex : Textures | std::views::values)
	{
		TextureVec.push_back(Tex);
	}

	// Add sphere for each texture for demo
	for (int Index = 0; Index < TextureVec.size(); ++Index)
	{
		const auto SphereActor = std::make_shared<Actor>("Sphere");

		auto SkeletalComp = std::make_shared<SkeletalMeshComponent>("SphereMesh", "Content/Meshes/sphere.obj");
		SkeletalComp->SetCustomTexture(TextureVec.at(Index));

		SphereActor->SetRootComponent(SkeletalComp);

		SphereActor->GetRootComponent()->SetLocation(-2.0f + static_cast<float>(Index) * 2, 0.05f, 5.0f);
		SphereActor->GetRootComponent()->SetScale(0.35f, 0.35f, 0.35f);
		SphereActor->GetRootComponent()->SetRotation(0.0f, 0.0f, 0.0f);

		AddActor(SphereActor);
	}
}