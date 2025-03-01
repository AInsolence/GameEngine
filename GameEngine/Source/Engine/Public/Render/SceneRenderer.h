#pragma once

#include <fwd.hpp>
#include <map>
#include <memory>
#include <vector>

#include "GL/glew.h"
#include "mat4x4.hpp"

#include "Core/Level.h"

class Shader;
class PointLight;
class DirectionalLight;
class Camera;
class MainWindow;

class SceneRenderer
{
public:
	SceneRenderer(std::shared_ptr<Level> LevelIns);

	void Initialize(const std::shared_ptr<MainWindow>& InGameWindow);

	void RenderScene(const std::shared_ptr<Camera>& PlayerCamera);

	std::shared_ptr<Level> GetLevelInstance() const;

private:
	std::shared_ptr<MainWindow> GameWindow;
	std::shared_ptr<Level> LevelInstance;

	void CreateShaders();

	void RenderStaticMeshes();
	void RenderPass(const std::shared_ptr<MainWindow>& MainWindow,
					const std::shared_ptr<Camera>& MainCamera,
					std::shared_ptr<DirectionalLight>& SunLight,
					const glm::mat4& ProjectionMatrix);

	void GenerateDirectionalShadowMaps(const std::shared_ptr<DirectionalLight>& Light);
	void GenerateOmniDirShadowMaps(const std::shared_ptr<PointLight>& Light);

	std::vector<std::unique_ptr<Shader>> ShaderList;
	std::unique_ptr<Shader> DirectionalShadowShader;
	std::shared_ptr<Shader> OmniDirectionalShadowShader;

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
};

