#include "Render/SceneRenderer.h"

#include <ranges>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

#include "Helper.h"
#include "Core/Actor.h"
#include "Components/Camera.h"
#include "Components/Grid.h"
#include "Components/Material.h"

#include "Render/Shader.h"
#include "Render/DirectionalLight.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Skybox.h"

#include "Core/MainWindow.h"
#include "Render/SpotLight.h"

SceneRenderer::SceneRenderer(std::shared_ptr<Level> LevelIns)  : LevelInstance(LevelIns)
{
}

void SceneRenderer::Initialize(const std::shared_ptr<MainWindow>& InGameWindow)
{
	GameWindow = InGameWindow;

	CreateShaders();

	ProjectionMatrix = glm::perspective(glm::radians(60.0f),
										static_cast<float>(GameWindow->GetBufferWidth()) / 
										static_cast<float>(GameWindow->GetBufferHeight()),
										0.1f, 100.0f);
}

void SceneRenderer::CreateShaders()
{
	ShaderList.emplace_back(std::make_unique<Shader>(VertexShaderPath, FragmentShaderPath));

	DirectionalShadowShader = std::make_unique<Shader>(VertDirShadowShaderPath, 
														FragDirShadowShaderPath);

	OmniDirectionalShadowShader = std::make_shared<Shader>(VertOmniDirShadowShaderPath, 
															GeomOmniDirShadowShaderPath, 
															FragOmniDirShadowShaderPath);
}

void SceneRenderer::RenderStaticMeshes()
{
	// Translation logic
	if (Direction)
	{
		ShapeOffset += ShapeOffsetStep;
	}
	else
	{
		ShapeOffset -= ShapeOffsetStep;
	}

	if(abs(ShapeOffset) >= ShapeMaxOffset)
	{
		Direction = !Direction;
	}

	// Rotation logic
	RotationDegree += RotationStep;

	if (RotationDegree >= 360.f)
	{
		RotationDegree = 0.0f;
	}

	// Scale logic
	if (ScaleDirection)
	{
		ScaleRatio += ScaleRatioStep;
	}
	else
	{
		ScaleRatio -= ScaleRatioStep;
	}

	if (abs(ScaleRatio) <= MinScaleRatio || abs(ScaleRatio) >= MaxScaleRatio)
	{
		ScaleDirection = !ScaleDirection;
	}

	// Set Model Translations
	glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix

	
	std::vector<std::shared_ptr<Texture>> TextureVec;
	for (auto& Tex : LevelInstance->GetTextures() | std::views::values)
	{
		TextureVec.push_back(Tex);
	}

	if (!LevelInstance->GetMeshList().empty())
	{
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(9.0f, -0.05f, -5.0f));
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		LevelInstance->GetTextures().at("Brick")->Apply();
		LevelInstance->GetMaterials().at("Mat")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		LevelInstance->GetMeshList()[0]->Render();
	}

	if (LevelInstance->GetMeshList().size() > 1)
	{
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f + ShapeOffset, 2.0f, -5.0f));
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		LevelInstance->GetTextures().at("Rock")->Apply();
		LevelInstance->GetMaterials().at("Metal")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		LevelInstance->GetMeshList()[1]->Render();
	}

	if (LevelInstance->GetMeshList().size() > 2)
	{
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(3.0f, -0.05f, -5.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationDegree), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		LevelInstance->GetTextures().at("Metal")->Apply();
		LevelInstance->GetMaterials().at("Metal")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		LevelInstance->GetMeshList()[2]->Render();
	}

	if (LevelInstance->GetMeshList().size() > 3)
	{
		// Set Model Translations
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, -1.0f, 0.0f)); // set translation
		
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		LevelInstance->GetTextures().at("Grid")->Apply();
		LevelInstance->GetMaterials().at("Mat")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		LevelInstance->GetMeshList()[3]->Render();
	}

	for (const auto& Actor : LevelInstance->GetAllActors())
	{
		if (!Actor) continue;

		const std::shared_ptr<SceneComponent> Root = Actor->GetRootComponent();

		if (!Root) continue;

		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		ModelMatrix = glm::translate(ModelMatrix, Root->GetLocation());
		ModelMatrix *= glm::mat4_cast(Root->GetRotation());
		ModelMatrix = glm::scale(ModelMatrix, Root->GetScale());

		for (const auto& ComponentPair : Actor->GetAllRenderableComponents()) 
		{
			auto RenderComponent = std::dynamic_pointer_cast<RenderableComponent>(ComponentPair);
			if (!RenderComponent) continue;

			// We take into account the offset if we have a child scene component
			glm::mat4 ComponentModelMatrix = ModelMatrix;

			const auto SceneComp = std::dynamic_pointer_cast<SceneComponent>(RenderComponent);

			if (std::dynamic_pointer_cast<SceneComponent>(RenderComponent)) 
			{
				ComponentModelMatrix = glm::translate(ComponentModelMatrix, SceneComp->GetLocation());
				ComponentModelMatrix *= glm::mat4_cast(SceneComp->GetRotation());
				ComponentModelMatrix = glm::scale(ComponentModelMatrix, SceneComp->GetScale());
			}

			glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ComponentModelMatrix));

			// TODO: switch to proper material instance applying
			LevelInstance->GetMaterials().at("Metal")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

			RenderComponent->Render();
		}
	}
}

void SceneRenderer::GenerateDirectionalShadowMaps(const std::shared_ptr<DirectionalLight>& Light)
{
	DirectionalShadowShader->Use();

	glViewport(0, 0, Light->GetShadowMap()->GetShadowWidth(), Light->GetShadowMap()->GetShadowHeight());

	Light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModelMatrix = DirectionalShadowShader->GetModelLocation();

	DirectionalShadowShader->SetDirectionalLightSpaceTransform(Light->CalculateLightSpaceTransform());

	DirectionalShadowShader->Validate();
	RenderStaticMeshes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::GenerateOmniDirShadowMaps(const std::shared_ptr<PointLight>& Light)
{
	OmniDirectionalShadowShader->Use();

	glViewport(0, 0, Light->GetShadowMap()->GetShadowWidth(), Light->GetShadowMap()->GetShadowHeight());

	Light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModelMatrix = OmniDirectionalShadowShader->GetModelLocation();
	UniformOmniLightPosition = OmniDirectionalShadowShader->GetUniformOmniLightPositionLocation();
	UniformFarPlane = OmniDirectionalShadowShader->GetUniformFarPlaneLocation();

	glUniform1f(UniformFarPlane, Light->GetFarPlane());
	glUniform3fv(UniformOmniLightPosition, 1, glm::value_ptr(Light->GetPosition()));

	OmniDirectionalShadowShader->SetLightMatrices(Light->CalculateLightSpaceTransform());

	OmniDirectionalShadowShader->Validate();
	RenderStaticMeshes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::RenderScene(const std::shared_ptr<Camera>& PlayerCamera)
{
	GenerateDirectionalShadowMaps(LevelInstance->GetSunLight());

	for (auto& Light : LevelInstance->GetPointLights())
	{
		GenerateOmniDirShadowMaps(Light);
	}

	for (auto& Light : LevelInstance->GetSpotLights())
	{
		GenerateOmniDirShadowMaps(Light);
	}

	RenderPass(GameWindow, PlayerCamera, LevelInstance->GetSunLight(), ProjectionMatrix);

	GameWindow->ShowFPS();

	glUseProgram(0);
	///* END of draw triangle *///
	GameWindow->SwapBuffers();
}

std::shared_ptr<Level> SceneRenderer::GetLevelInstance() const
{
	return LevelInstance;
}

void SceneRenderer::RenderPass(const std::shared_ptr<MainWindow>& MainWindow,
								const std::shared_ptr<Camera>& MainCamera,
								const std::shared_ptr<DirectionalLight>& SunLight,
								const glm::mat4& ProjectionMatrix)
{
	glViewport(0, 0, MainWindow->GetBufferWidth(), MainWindow->GetBufferHeight());

	// Clear window with black color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	LevelInstance->GetSkybox()->DrawSky(ProjectionMatrix, MainCamera->GetViewMatrix());

	LevelInstance->GetEditorGrid()->Draw(ProjectionMatrix, MainCamera->GetViewMatrix());

	///* Use shader program *///
	ShaderList[0]->Use();
	
	UniformDirectionalLightSpecularIntensity = ShaderList[0]->GetDirectionalLightSpecularIntensityLocation();
	UniformDirectionalLightShininess = ShaderList[0]->GetDirectionalLightShininessLocation();
	UniformModelMatrix = ShaderList[0]->GetModelLocation();
	UniformCameraPosition = ShaderList[0]->GetUniformCameraPositionLocation();
	
	// Set Projection matrix
	glUniformMatrix4fv(ShaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	// Set view matrix
	glUniformMatrix4fv(ShaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(MainCamera->GetViewMatrix()));
	// Set Camera Position
	glUniform3fv(UniformCameraPosition, 1, glm::value_ptr(MainCamera->GetPosition()));
	
	ShaderList[0]->SetDirectionalLight(LevelInstance->GetSunLight());
	ShaderList[0]->SetDirectionalLightSpaceTransform(LevelInstance->GetSunLight()->CalculateLightSpaceTransform());

	LevelInstance->GetSunLight()->GetShadowMap()->Read(GL_TEXTURE3);
	ShaderList.at(0)->SetTextureUnit(2);
	ShaderList.at(0)->SetDirectionalShadowMap(3);

	ShaderList.at(0)->SetPointLights(LevelInstance->GetPointLights(), 4, 0);
	ShaderList.at(0)->SetSpotLights(LevelInstance->GetSpotLights(), 4 + LevelInstance->GetPointLights().size(), LevelInstance->GetPointLights().size());

	glm::vec3 HandsPosition = MainCamera->GetPosition();
	HandsPosition.y -= 0.1f;

	if (!LevelInstance->GetSpotLights().empty())
	{
		LevelInstance->GetSpotLights().at(0)->SetTransform(HandsPosition, MainCamera->GetDirection());
	}

	ShaderList.at(0)->Validate();

	RenderStaticMeshes();
}
