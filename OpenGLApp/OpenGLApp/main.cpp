#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <memory>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "MainWindow.h"
#include "Mesh.h"
#include "Shader.h"

// GL window dimension
constexpr GLint WIDTH = 800;
constexpr GLint HIGHT = 600;

constexpr float ToRadians = 3.14159265f / 180.0f;

// Define main variables
std::vector<std::shared_ptr<Mesh>> MeshList;
std::vector<std::shared_ptr<Shader>> ShaderList;

bool Direction = true;
float ShapeOffset = 0.0f;
float ShapeMaxOffset = 0.6f;
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
	// Represents faces created from further represented vertices
	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Define triangle's vertices
	constexpr GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0,  0.0f
	};

	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices, 12, 12));
	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices, 12, 12));
	MeshList.emplace_back(std::make_shared<Mesh>(Vertices, Indices, 12, 12));
}

int main()
{
	MainWindow MainWindow (800, 800);

	Create3DObjects();
	ShaderList.emplace_back(std::make_shared<Shader>(VertexShaderPath, FragmentShaderPath));

	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, MainWindow.GetBufferWidth() / MainWindow.GetBufferHeight(), 0.1f, 100.0f); // initialize projection matrix

	// Render loop
	while (!MainWindow.GetShouldClose())
	{
		// Get and handle user input events
		glfwPollEvents();

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

		///* Draw triangle *///
		ShaderList[0]->Use();

		// Set Projection matrix
		glUniformMatrix4fv(ShaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		// Get Model Matrix location
		const GLint UniformModelMatrix_id = ShaderList[0]->GetModelLocation();

		if (!MeshList.empty())
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(ShapeOffset, ShapeOffset, -2.5f)); // set translation
			ModelMatrix = glm::rotate(ModelMatrix, RotationDegree * ToRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

			MeshList[0]->Render();
		}

		if (MeshList.size() > 1)
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-ShapeOffset, ShapeOffset, -2.5f)); // set translation
			ModelMatrix = glm::rotate(ModelMatrix, RotationDegree * ToRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

			MeshList[1]->Render();
		}

		if (MeshList.size() > 2)
		{
			// Set Model Translations
			glm::mat4 ModelMatrix (1.0f); // initialize module matrix as identity matrix
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-ShapeOffset, -ShapeOffset, -2.5f)); // set translation
			ModelMatrix = glm::rotate(ModelMatrix, RotationDegree * ToRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // set rotation
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(ScaleRatio, ScaleRatio, ScaleRatio)); // set scale

			glUniformMatrix4fv(UniformModelMatrix_id, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

			MeshList[2]->Render();
		}

		glUseProgram(0);
		///* END of draw triangle *///
		MainWindow.SwapBuffers();
	}

	return 0;
}