#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <map>
#include <memory>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Core/MainWindow.h"

#include "Render/Shader.h"
#include "Render/SpotLight.h"
#include "Render/PointLight.h"
#include "Render/DirectionalLight.h"

#include "Components/Mesh.h"
#include "Components/Camera.h"
#include "Components/Skybox.h"
#include "Components/Texture.h"
#include "Components/Material.h"
#include "Components/SkeletalMesh.h"

#include "Utilities/Helper.h"
#include "Components/Grid.h"


// Define main variables
std::unique_ptr<Grid> EditorGrid;
std::unique_ptr<Skybox> Sky;
std::vector<std::unique_ptr<Mesh>> MeshList;
std::vector<std::unique_ptr<SkeletalMesh>> SkeletalMeshList;
std::vector<std::unique_ptr<Shader>> ShaderList;
std::unique_ptr<Shader> DirectionalShadowShader;
std::unique_ptr<Shader> OmniDirectionalShadowShader;

std::vector<PointLight> PointLights;
std::vector<SpotLight> SpotLights;


std::map<const char*, std::unique_ptr<Texture>> Textures;
std::map<const char*, std::unique_ptr<Material>> Materials;

// DeltaTime TODO: move to World class
GLfloat DeltaTime = 0.0f;
GLfloat LastTime = 0.0f;

bool Direction = true;
float ShapeOffset = 0.0f;
float ShapeMaxOffset = 3.5f;
float ShapeOffsetStep = 0.005f;

float RotationDegree = 0.0f;

float ScaleRatio = 0.1f;
float MinScaleRatio = 0.1f;
float MaxScaleRatio = 0.3f;
float ScaleRatioStep = 0.001f;
bool ScaleDirection = true;


// Main Shader code
static const char* VertexShaderPath = "Source/Engine/Public/Render/Shaders/shader.vert";
static const char* FragmentShaderPath = "Source/Engine/Public/Render/Shaders/shader.frag";
// Shadow Shader code
static const char* VertDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/directional_shadow_map_shader.vert";
static const char* FragDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/directional_shadow_map_shader.frag";
// Omni Shadow shader
static const char* VertOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.vert";
static const char* GeomOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.geom";
static const char* FragOmniDirShadowShaderPath = "Source/Engine/Public/Render/Shaders/omni_shadow_map_shader.frag";

GLint UniformModelMatrix;
GLint UniformDirectionalLightSpecularIntensity;
GLint UniformDirectionalLightShininess;
GLint UniformCameraPosition;
GLint UniformOmniLightPosition;
GLint UniformFarPlane;
		

void CreateShaders()
{
	ShaderList.emplace_back(std::make_unique<Shader>(VertexShaderPath, FragmentShaderPath));

	DirectionalShadowShader = std::make_unique<Shader>(VertDirShadowShaderPath, 
														FragDirShadowShaderPath);

	OmniDirectionalShadowShader = std::make_unique<Shader>(VertOmniDirShadowShaderPath, 
															GeomOmniDirShadowShaderPath, 
															FragOmniDirShadowShaderPath);
}

void LoadTextures()
{
	PlaceholderTexture->LoadTexture_RGBA();

	Textures.emplace("Brick", std::make_unique<Texture>("Content/Textures/brick.jpg"));
	Textures.emplace("Rock", std::make_unique<Texture>("Content/Textures/rock.jpg"));
	Textures.emplace("Metal", std::make_unique<Texture>("Content/Textures/metal.jpg"));
	Textures.emplace("Sand", std::make_unique<Texture>("Content/Textures/sand.png"));
	Textures.emplace("Gold", std::make_unique<Texture>("Content/Textures/gold.jpg"));
	Textures.emplace("Grid", std::make_unique<Texture>("Content/Textures/small_grid.png"));
	Textures.emplace("Plain", std::make_unique<Texture>("Content/Textures/plain.png"));

	for (const auto& Texture : Textures)
	{
		Texture.second->LoadTexture_RGBA();
	}
}

void LoadMaterials()
{
	Materials.emplace("Metal", std::make_unique<Material>(5.0f, 128.0f));
	Materials.emplace("Mat", std::make_unique<Material>(0.5f, 1.0f));
}

void CreateSkybox()
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

void Create3DObjects()
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

	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(FloorVertices, FloorIndices));

	SkeletalMeshList.emplace_back(std::make_unique<SkeletalMesh>("Content/Meshes/Pony_cartoon.obj"));
	//SkeletalMeshList.emplace_back(std::make_unique<SkeletalMesh>("Content/Meshes/scene.gltf"));
}

void RenderScene()
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

void DirectionalShadowMapPass(DirectionalLight& SunLight)
{
	DirectionalShadowShader->Use();

	glViewport(0, 0, SunLight.GetShadowMap()->GetShadowWidth(), SunLight.GetShadowMap()->GetShadowHeight());

	SunLight.GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModelMatrix = DirectionalShadowShader->GetModelLocation();

	DirectionalShadowShader->SetDirectionalLightSpaceTransform(SunLight.CalculateLightSpaceTransform());

	DirectionalShadowShader->Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniDirectionalShadowMapPass(PointLight& Light)
{
	OmniDirectionalShadowShader->Use();

	glViewport(0, 0, Light.GetShadowMap()->GetShadowWidth(), Light.GetShadowMap()->GetShadowHeight());

	Light.GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	UniformModelMatrix = OmniDirectionalShadowShader->GetModelLocation();
	UniformOmniLightPosition = OmniDirectionalShadowShader->GetUniformOmniLightPositionLocation();
	UniformFarPlane = OmniDirectionalShadowShader->GetUniformFarPlaneLocation();

	glUniform1f(UniformFarPlane, Light.GetFarPlane());
	glUniform3fv(UniformOmniLightPosition, 1, glm::value_ptr(Light.GetPosition()));

	OmniDirectionalShadowShader->SetLightMatrices(Light.CalculateLightSpaceTransform());

	OmniDirectionalShadowShader->Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render(const MainWindow& MainWindow,
			const Camera& MainCamera,
			DirectionalLight& SunLight,
			const glm::mat4& ProjectionMatrix)
{
	glViewport(0, 0, MainWindow.GetBufferWidth(), MainWindow.GetBufferHeight());

	// Clear window with black color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Sky->DrawSky(ProjectionMatrix, MainCamera.GetViewMatrix());

	EditorGrid->Draw(ProjectionMatrix, MainCamera.GetViewMatrix());

	///* Use shader program *///
	ShaderList[0]->Use();
	
	UniformDirectionalLightSpecularIntensity = ShaderList[0]->GetDirectionalLightSpecularIntensityLocation();
	UniformDirectionalLightShininess = ShaderList[0]->GetDirectionalLightShininessLocation();
	UniformModelMatrix = ShaderList[0]->GetModelLocation();
	UniformCameraPosition = ShaderList[0]->GetUniformCameraPositionLocation();
	
	// Set Projection matrix
	glUniformMatrix4fv(ShaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	// Set view matrix
	glUniformMatrix4fv(ShaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(MainCamera.GetViewMatrix()));
	// Set Camera Position
	glUniform3fv(UniformCameraPosition, 1, glm::value_ptr(MainCamera.GetPosition()));
	
	ShaderList[0]->SetDirectionalLight(SunLight);
	ShaderList[0]->SetDirectionalLightSpaceTransform(SunLight.CalculateLightSpaceTransform());

	SunLight.GetShadowMap()->Read(GL_TEXTURE3);
	ShaderList.at(0)->SetTextureUnit(2);
	ShaderList.at(0)->SetDirectionalShadowMap(3);

	ShaderList[0]->SetPointLights(PointLights, 4, 0);
	ShaderList[0]->SetSpotLights(SpotLights, 4 + PointLights.size(), PointLights.size());

	glm::vec3 HandsPosition = MainCamera.GetPosition();
	HandsPosition.y -= 0.1f;

	SpotLights[0].SetTransform(HandsPosition, MainCamera.GetDirection());

	ShaderList[0]->Validate();

	RenderScene();
}

int main()
{
	MainWindow MainWindow (2400, 1350);
	Camera MainCamera(glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f),
						-90.0f,
						0.0f,
						0.0f,
						5.0f,
						0.2f);

	LoadTextures();
	LoadMaterials();

	Create3DObjects();
	CreateShaders();

	CreateSkybox();

	auto SunLight = DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
									0.3f, 
									0.8f,
									glm::normalize(glm::vec3(-2.0f, -1.0f, 0.3f)),
									4096, 4096);


	PointLights.emplace_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
							0.0f, 4.0f,
							glm::vec3(0.0f, 0.8f, -4.0f),
							0.3f, 0.2f, 0.1f,
							3.0f, 15.0f, 10.0f);

	PointLights.emplace_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
							0.0f, 4.0f,
							glm::vec3(8.0f, 0.8f, -4.0f),
							0.3f, 0.2f, 0.1f,
							5.0f, 15.0f, 15.0f);


	SpotLights.emplace_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
							0.0f, 2.0f,
							glm::vec3(0.0f, 0.8f, 3.0f),
							glm::vec3(0.0f, 0.8f, 3.0f),
							20.0f,
							0.3f, 0.2f, 0.1f,
							4.0f, 10.0f, 10.0f);;

	// Initialize projection matrix
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(60.0f),
										static_cast<float>(MainWindow.GetBufferWidth()) / 
													static_cast<float>(MainWindow.GetBufferHeight()),
													0.1f, 100.0f);

	// Render loop
	while (!MainWindow.GetShouldClose())
	{
		GLfloat Now = glfwGetTime(); // SDL_GetPerformanceCounter();
		DeltaTime = Now - LastTime; // (now - LastTime)*1000/SDL_GetPerformanceFrequency();
		LastTime = Now;

		// Get and handle user input events
		glfwPollEvents();

		// Transfer inputs from main window to camera
		MainCamera.KeyControl(MainWindow.GetKeys(), DeltaTime);
		MainCamera.MouseControl(MainWindow.GetOffsetX(), MainWindow.GetOffsetY());

		if (MainWindow.GetKeys()[GLFW_KEY_L])
		{
			SpotLights[0].Toggle();
			MainWindow.GetKeys()[GLFW_KEY_L] = false;
		}

		DirectionalShadowMapPass(SunLight);

		for (auto& Light : PointLights)
		{
			OmniDirectionalShadowMapPass(Light);
		}

		for (auto& Light : SpotLights)
		{
			OmniDirectionalShadowMapPass(Light);
		}

		Render(MainWindow, MainCamera, SunLight, ProjectionMatrix);

		glUseProgram(0);
		///* END of draw triangle *///
		MainWindow.SwapBuffers();
	}

	return 0;
}