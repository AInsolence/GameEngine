#include "Core/World.h"

#include <array>
#include <gtc/type_ptr.hpp>

#include "Utilities/Helper.h"

World::World()
{
	Initialize();
}

void World::ProcessInput(GLfloat DeltaTime) const
{
	for (const auto& Controller : Controllers)
	{
		Controller->HandleInput(DeltaTime);
	}
}

void World::Initialize()
{
	GameWindow = std::make_shared<MainWindow>(2400, 1350);
	PlayerCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 
											glm::vec3(0.0f, 1.0f, 0.0f),
											0.0f,
											0.0f,
											0.0f,
											5.0f,
											0.2f);

	LoadTextures();
	LoadMaterials();

	Create3DObjects();
	CreateShaders();

	CreateSkybox();

	SunLight = std::make_shared<DirectionalLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
													0.3f, 
													0.8f,
													glm::normalize(glm::vec3(-2.0f, -1.0f, 0.3f)),
													4096, 4096);


	PointLights.emplace_back(std::make_shared<PointLight>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
							 0.0f, 4.0f,
							 glm::vec3(0.0f, 0.8f, -4.0f),
							 0.3f, 0.2f, 0.1f,
							 3.0f, 15.0f, 10.0f));

	PointLights.emplace_back(std::make_shared<PointLight>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
							0.0f, 4.0f,
							glm::vec3(8.0f, 0.8f, -4.0f),
							0.3f, 0.2f, 0.1f,
							5.0f, 15.0f, 15.0f));


	PointLights.emplace_back(std::make_shared<PointLight>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
							0.0f, 4.0f,
							glm::vec3(4.0f, 0.8f, -4.0f),
							0.3f, 0.2f, 0.1f,
							6.0f, 15.0f, 15.0f));

	SpotLights.emplace_back(std::make_shared<SpotLight>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
							0.0f, 2.0f,
							glm::vec3(0.0f, 0.8f, 3.0f),
							glm::vec3(0.0f, 0.0f, 0.0f),
							20.0f,
							0.3f, 0.2f, 0.1f,
							4.0f, 10.0f, 10.0f));

	

	// Initialize projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.0f),
										static_cast<float>(GameWindow->GetBufferWidth()) / 
										static_cast<float>(GameWindow->GetBufferHeight()),
										0.1f, 100.0f);

	FirstPlayerController = std::make_shared<PlayerController>(GameWindow, PlayerCamera);

	// TODO: temp for debug purposes
	if (!SpotLights.empty())
	{
		FirstPlayerController->SetFlashLight(SpotLights.at(0));
	}

	RegisterController(FirstPlayerController);
	
}

bool World::IsGameOver()
{
	return GameWindow->GetShouldClose();
}

void World::RegisterController(const std::shared_ptr<IInputController>& Controller)
{
	Controllers.push_back(Controller);
}

void World::CreateShaders()
{
	ShaderList.emplace_back(std::make_unique<Shader>(VertexShaderPath, FragmentShaderPath));

	DirectionalShadowShader = std::make_unique<Shader>(VertDirShadowShaderPath, 
														FragDirShadowShaderPath);

	OmniDirectionalShadowShader = std::make_shared<Shader>(VertOmniDirShadowShaderPath, 
															GeomOmniDirShadowShaderPath, 
															FragOmniDirShadowShaderPath);
}

void World::LoadTextures()
{
	PlaceholderTexture->LoadTexture_RGBA();

	Textures.emplace("Brick", std::make_shared<Texture>("Content/Textures/brick.jpg"));
	Textures.emplace("Rock", std::make_shared<Texture>("Content/Textures/rock.jpg"));
	Textures.emplace("Metal", std::make_shared<Texture>("Content/Textures/metal.jpg"));
	Textures.emplace("Sand", std::make_shared<Texture>("Content/Textures/sand.png"));
	Textures.emplace("Gold", std::make_shared<Texture>("Content/Textures/gold.jpg"));
	Textures.emplace("Grid", std::make_shared<Texture>("Content/Textures/small_grid.png"));
	Textures.emplace("Plain", std::make_shared<Texture>("Content/Textures/plain.png"));

	for (const auto& Texture : Textures)
	{
		Texture.second->LoadTexture_RGBA();
	}
}

void World::LoadMaterials()
{
	Materials.emplace("Metal", std::make_shared<Material>(5.0f, 128.0f));
	Materials.emplace("Mat", std::make_shared<Material>(0.5f, 1.0f));
}

void World::CreateSkybox()
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

void World::Create3DObjects()
{
	EditorGrid = std::make_unique<Grid>(100, 50);

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

	SkeletalMeshList.emplace_back(std::make_shared<SkeletalMesh>("Content/Meshes/Pony_cartoon.obj"));
	//SkeletalMeshList.emplace_back(std::make_shared<SkeletalMesh>("Content/Meshes/scene.gltf"));
}

void World::RenderStaticMeshes()
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
	RotationDegree += 0.5f;

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

	if (!SkeletalMeshList.empty())
	{
		// Set Model Translations
		ModelMatrix = 1.0f; // initialize module matrix as identity matrix
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, -1.05f, -3.0f)); // set translation
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.004f, 0.004f, 0.004f)); // set scale
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		SkeletalMeshList.at(0)->Render();
	}

	//if (!SkeletalMeshList.empty())
	//{
	//	// Set Model Translations
	//	ModelMatrix = 1.0f; // initialize module matrix as identity matrix
	//	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1.0f, -1.05f, 3.0f)); // set translation
	//	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.006f, 0.006f, 0.006f)); // set scale
	//	//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // set rotation

	//	glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	//	SkeletalMeshList.at(1)->Render();
	//}

	if (!MeshList.empty())
	{
		// Set Model Translations
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(8.0f, 0.0f, 0.0f)); // set translation

		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		Textures.at("Brick")->Apply();
		Materials.at("Mat")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		MeshList[0]->Render();
	}

	if (MeshList.size() > 1)
	{
		// Set Model Translations
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f + ShapeOffset, 2.0f, 0.0f)); // set translation
		
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		Textures.at("Rock")->Apply();
		Materials.at("Metal")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		MeshList[1]->Render();
	}

	if (MeshList.size() > 2)
	{
		// Set Model Translations
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, -0.05f, 0.0f)); // set translation
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationDegree), glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation

		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		Textures.at("Metal")->Apply();
		Materials.at("Metal")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		MeshList[2]->Render();
	}

	if (MeshList.size() > 3)
	{
		// Set Model Translations
		ModelMatrix = 1.0f;
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, -1.0f, 0.0f)); // set translation
		
		glUniformMatrix4fv(UniformModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		Textures.at("Grid")->Apply();
		Materials.at("Mat")->Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

		MeshList[3]->Render();
	}
}

void World::GenerateDirectionalShadowMaps(const std::shared_ptr<DirectionalLight>& Light)
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

void World::GenerateOmniDirShadowMaps(const std::shared_ptr<PointLight>& Light)
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

void World::RenderScene()
{
	GenerateDirectionalShadowMaps(SunLight);

	for (auto& Light : PointLights)
	{
		GenerateOmniDirShadowMaps(Light);
	}

	for (auto& Light : SpotLights)
	{
		GenerateOmniDirShadowMaps(Light);
	}

	RenderPass(GameWindow, PlayerCamera, SunLight, ProjectionMatrix);

	glUseProgram(0);
	///* END of draw triangle *///
	GameWindow->SwapBuffers();
}

void World::RenderPass(const std::shared_ptr<MainWindow>& MainWindow,
							const std::shared_ptr<Camera>& MainCamera,
							std::shared_ptr<DirectionalLight>& SunLight,
							const glm::mat4& ProjectionMatrix)
{
	glViewport(0, 0, MainWindow->GetBufferWidth(), MainWindow->GetBufferHeight());

	// Clear window with black color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Sky->DrawSky(ProjectionMatrix, MainCamera->GetViewMatrix());

	EditorGrid->Draw(ProjectionMatrix, MainCamera->GetViewMatrix());

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
	
	ShaderList[0]->SetDirectionalLight(SunLight);
	ShaderList[0]->SetDirectionalLightSpaceTransform(SunLight->CalculateLightSpaceTransform());

	SunLight->GetShadowMap()->Read(GL_TEXTURE3);
	ShaderList.at(0)->SetTextureUnit(2);
	ShaderList.at(0)->SetDirectionalShadowMap(3);

	ShaderList.at(0)->SetPointLights(PointLights, 4, 0);
	ShaderList.at(0)->SetSpotLights(SpotLights, 4 + PointLights.size(), PointLights.size());

	glm::vec3 HandsPosition = MainCamera->GetPosition();
	HandsPosition.y -= 0.1f;

	if (!SpotLights.empty())
	{
		SpotLights.at(0)->SetTransform(HandsPosition, MainCamera->GetDirection());
	}

	ShaderList.at(0)->Validate();

	RenderStaticMeshes();
}
