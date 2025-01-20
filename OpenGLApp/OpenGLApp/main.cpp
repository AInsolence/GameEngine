#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <memory>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "MainWindow.h"
#include "Mesh.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Shader.h"
#include "SkeletalMesh.h"
#include "Texture.h"

#include "StaticHelper.h"


// Define main variables
std::vector<std::unique_ptr<Mesh>> MeshList;
std::vector<std::unique_ptr<Shader>> ShaderList;

// DeltaTime TODO: move to World class
GLfloat DeltaTime = 0.0;
GLfloat LastTime = 0.0;

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


// Vertex Shader code
static const char* VertexShaderPath = "Shaders/shader.vert";

// Fragment Shader code
static const char* FragmentShaderPath = "Shaders/shader.frag";



void Create3DObjects()
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

	StaticHelper::CalculateAverageNormals(Indices, Vertices, 8, 5);

	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(Vertices, Indices));
	MeshList.emplace_back(std::make_unique<Mesh>(FloorVertices, FloorIndices));
}

int main()
{
	MainWindow MainWindow (1366, 768);
	Camera MainCamera(glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f),
						-90.0f,
						0.0f,
						0.0f,
						5.0f,
						0.2f);

	Create3DObjects();
	auto Model = SkeletalMesh("Content/Meshes/Pony_cartoon.obj");
	//auto Model_2 = SkeletalMesh("Content/Meshes/geo_dead.obj");

	auto BrickTexture = Texture("Content/Textures/brick.jpg");
	BrickTexture.LoadTexture_RGBA();
	auto RockTexture = Texture("Content/Textures/rock.jpg");
	RockTexture.LoadTexture_RGBA();
	auto MetalTexture = Texture("Content/Textures/metal.jpg");
	MetalTexture.LoadTexture_RGBA();
	auto SandTexture = Texture("Content/Textures/sand.png");
	SandTexture.LoadTexture_RGBA();
	auto GoldTexture = Texture("Content/Textures/gold.jpg");
	GoldTexture.LoadTexture_RGBA();

	auto MetalMaterial = Material(5.0f, 128.0f);
	auto MatMaterial = Material(0.5f, 1.0f);

	auto SunLight = DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
									0.2f, 
									0.8f,
									glm::normalize(glm::vec3(2.0f, 1.0f, 0.0f)));

	std::vector<PointLight> PointLights;

	PointLights.emplace_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
							0.0f, 2.0f,
							glm::vec3(0.0f, 0.8f, 3.0f),
							0.3f, 0.2f, 0.1f,
							3.0f, 15.0f, 10.0f);

	PointLights.emplace_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
							0.0f, 2.0f,
							glm::vec3(8.0f, 0.8f, 3.0f),
							0.3f, 0.2f, 0.1f,
							5.0f, 5.0f, 15.0f);

	std::vector<SpotLight> SpotLights;

	SpotLights.emplace_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
							0.0f, 1.0f,
							glm::vec3(0.0f, 0.8f, 3.0f),
							glm::vec3(0.0f, 0.8f, 3.0f),
							20.0f,
							0.3f, 0.2f, 0.1f,
							3.0f, 15.0f, 10.0f);;

	ShaderList.emplace_back(std::make_unique<Shader>(VertexShaderPath, FragmentShaderPath));

	// Initialize projection matrix
	glm::mat4 ProjectionMatrix = glm::perspective(45.0f,
									static_cast<float>(MainWindow.GetBufferWidth()) / static_cast<float>(MainWindow.GetBufferHeight()),
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

		// Clear window with black color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		///* Use shader program *///
		ShaderList[0]->Use();

		// Get Model Matrix location
		const GLint UniformModelMatrix_id = ShaderList[0]->GetModelLocation();
		// Get DirectionalLight locations
		const GLint UniformDirectionalLightSpecularIntensity = ShaderList[0]->GetDirectionalLightSpecularIntensityLocation();
		const GLint UniformDirectionalLightShininess = ShaderList[0]->GetDirectionalLightShininessLocation();
		const GLint UniformCameraPosition = ShaderList[0]->GetUniformCameraPositionLocation();
		
		// Set Projection matrix
		glUniformMatrix4fv(ShaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		// Set view matrix
		glUniformMatrix4fv(ShaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(MainCamera.GetViewMatrix()));
		// Set Camera Position
		glUniform3fv(UniformCameraPosition, 1, glm::value_ptr(MainCamera.GetPosition()));


		glm::vec3 HandsPosition = MainCamera.GetPosition();
		HandsPosition.y -= 0.1f;
		SpotLights[0].SetTransform(HandsPosition, MainCamera.GetDirection());

		ShaderList[0]->SetDirectionalLight(SunLight);
		ShaderList[0]->SetPointLights(PointLights);
		ShaderList[0]->SetSpotLights(SpotLights);

		//// Set Model Translations
		glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(4.0f, -1.05f, 3.0f)); // set translation
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.004f, 0.004f, 0.004f)); // set scale
		glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		Model.Render();

		if (!MeshList.empty())
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 0.0f, 0.0f)); // set translation
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationDegree), glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			BrickTexture.Apply();
			MatMaterial.Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

			MeshList[0]->Render();
		}

		if (MeshList.size() > 1)
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f + ShapeOffset, 2.0f, 0.0f)); // set translation
			//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationDegree), glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			RockTexture.Apply();
			MetalMaterial.Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

			MeshList[1]->Render();
		}

		if (MeshList.size() > 2)
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(8.0f, 0.0f, 0.0f)); // set translation
			ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationDegree), glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			MetalTexture.Apply();
			MetalMaterial.Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

			MeshList[2]->Render();
		}

		if (MeshList.size() > 3)
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(5.0f, -1.0f, 0.0f)); // set translation
			
			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
			SandTexture.Apply();
			MatMaterial.Apply(UniformDirectionalLightSpecularIntensity, UniformDirectionalLightShininess);

			MeshList[3]->Render();
		}

		glUseProgram(0);
		///* END of draw triangle *///
		MainWindow.SwapBuffers();
	}

	return 0;
}