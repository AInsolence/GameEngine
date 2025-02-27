#pragma once
#include <map>
#include <memory>
#include <vector>

#include "Render/Shader.h"
#include "Core/MainWindow.h"
#include "Components/Grid.h"
#include "Components/Mesh.h"
#include "Render/SpotLight.h"
#include "Components/Skybox.h"
#include "Components/Camera.h"
#include "Components/Material.h"
#include "Core/PlayerController.h"
#include "Render/DirectionalLight.h"
#include "Components/SkeletalMeshComponent.h"

class World
{
public:
	World();
	void Initialize();

	bool IsGameOver();
	void RenderScene();
	void ProcessInput(GLfloat DeltaTime) const;

	void RegisterController(const std::shared_ptr<IInputController>& Controller);

private:
	void LoadTextures();
	void LoadMaterials();

	void CreateSkybox();
	void CreateShaders();
	void Create3DObjects();
	void CreateLight();

	void RenderStaticMeshes();
	void RenderPass(const std::shared_ptr<MainWindow>& MainWindow,
					const std::shared_ptr<Camera>& MainCamera,
					std::shared_ptr<DirectionalLight>& SunLight,
					const glm::mat4& ProjectionMatrix);

	void GenerateDirectionalShadowMaps(const std::shared_ptr<DirectionalLight>& Light);
	void GenerateOmniDirShadowMaps(const std::shared_ptr<PointLight>& Light);

	std::shared_ptr<MainWindow> GameWindow;
	std::shared_ptr<Camera> PlayerCamera;
	std::shared_ptr<PlayerController> FirstPlayerController;

	std::unique_ptr<Skybox> Sky;
	std::unique_ptr<Grid> EditorGrid;

	std::vector<std::shared_ptr<Mesh>> MeshList;
	std::vector<std::shared_ptr<SkeletalMeshComponent>> SkeletalMeshList;
	std::shared_ptr<SkeletalMeshComponent> PonyCar;

	std::vector<std::unique_ptr<Shader>> ShaderList;
	std::unique_ptr<Shader> DirectionalShadowShader;
	std::shared_ptr<Shader> OmniDirectionalShadowShader;

	std::shared_ptr<DirectionalLight> SunLight;
	std::vector<std::shared_ptr<PointLight>> PointLights;
	std::vector<std::shared_ptr<SpotLight>> SpotLights;

	std::map<const char*, std::shared_ptr<Texture>> Textures;
	std::map<const char*, std::shared_ptr<Material>> Materials;

	glm::mat4 ProjectionMatrix;

	bool Direction = true;
	float ShapeOffset = 0.0f;
	float ShapeMaxOffset = 3.5f;
	float ShapeOffsetStep = 0.002f;

	float RotationDegree = 0.0f;

	float ScaleRatio = 0.1f;
	float MinScaleRatio = 0.1f;
	float MaxScaleRatio = 0.3f;
	float ScaleRatioStep = 0.001f;
	float RotationStep = 0.2f;
	bool ScaleDirection = true;

	// Main Shader code
	const char* VertexShaderPath = "Source/Engine/Public/Render/Shaders/shader.vert";
	const char* FragmentShaderPath = "Source/Engine/Public/Render/Shaders/shader.frag";
	// Shadow Shader code
	const char* VertDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/directional_shadow_map_shader.vert";
	const char* FragDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/directional_shadow_map_shader.frag";
	// Omni Shadow shader
	const char* VertOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.vert";
	const char* GeomOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.geom";
	const char* FragOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.frag";

	GLint UniformModelMatrix;
	GLint UniformDirectionalLightSpecularIntensity;
	GLint UniformDirectionalLightShininess;
	GLint UniformCameraPosition;
	GLint UniformOmniLightPosition;
	GLint UniformFarPlane;

	std::vector<std::shared_ptr<IInputController>> Controllers;
};

//#pragma once
//#include <memory>
//#include "Scene.h"
//#include "SceneRenderer.h"
//#include "Core/PlayerController.h"
//
//class World {
//public:
//	World();
//	void Initialize();
//
//	void ProcessInput(GLfloat DeltaTime) const;
//	void Update(GLfloat DeltaTime);
//	void Render();
//
//	void RegisterController(const std::shared_ptr<IInputController>& Controller);
//
//private:
//	std::shared_ptr<Scene> GameScene;
//	std::shared_ptr<SceneRenderer> SceneRender;
//	std::shared_ptr<Camera> PlayerCamera;
//	std::shared_ptr<PlayerController> FirstPlayerController;
//};