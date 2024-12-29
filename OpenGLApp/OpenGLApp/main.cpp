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

#include "Mesh.h"

// GL window dimension
constexpr GLint WIDTH = 800;
constexpr GLint HIGHT = 600;

constexpr float ToRadians = 3.14159265f / 180.0f;

// Define main variables

std::vector<std::shared_ptr<Mesh>> MeshList;

GLuint ShaderProgram_id;
GLuint UniformXMoveOffsetVar_id;
GLuint UniformModelMatrix_id;
GLuint UniformProjectionMatrix_id;

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
static const char* VertexShaderCode = R"gl(

	#version 330

	layout (location = 0) in vec3 pos;

	uniform float XMoveOffset;
	uniform mat4 ModelMatrix;
	uniform mat4 ProjectionMatrix;

	noperspective out vec4 VertexColor;

	void main()
	{
		gl_Position = ProjectionMatrix * ModelMatrix * vec4(pos, 1.0);
		VertexColor = vec4(clamp(pos, 0.3f, 1.0f), 1.0f);
	}

)gl";

// Fragment Shader code
static const char* FragmentShaderCode = R"gl(

	#version 330

	in vec4 VertexColor;
	out vec4 color;

	void main()
	{
		color = VertexColor;
	}

)gl";

// Function:
//  1) Created shader from shader source code
//  2) Compile it and check compilation status
//  3) Attach it to shader program
void AddShader(GLuint ShaderProgramId, const char* ShaderCode, GLenum ShaderType)
{
	// Create shader
	const GLuint LocalShader = glCreateShader(ShaderType);

	// Type conversion to GL types
	const GLchar* LocalShaderCode[1];
	LocalShaderCode[0] = ShaderCode;

	GLint CodeLength [1];
	CodeLength[0] = strlen(ShaderCode);

	// Set shader source and compile shader
	glShaderSource(LocalShader, 1, LocalShaderCode, CodeLength);
	glCompileShader(LocalShader);

	// Check shader compilation status
	GLint Result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(LocalShader, GL_COMPILE_STATUS, &Result);

	if (!Result)
	{
		glGetShaderInfoLog(LocalShader, sizeof(eLog), nullptr, eLog);
		printf("Error compiling the %d shader: '%s'\n", ShaderType, eLog);
		return;
	}
	//

	// Attach compiled shader to the shader program
	glAttachShader(ShaderProgramId, LocalShader);
}

void CompileShaders()
{
	ShaderProgram_id = glCreateProgram();

	if (!ShaderProgram_id)
	{
		printf("Error creating shader program");
		return;
	}

	AddShader(ShaderProgram_id, VertexShaderCode, GL_VERTEX_SHADER);
	AddShader(ShaderProgram_id, FragmentShaderCode, GL_FRAGMENT_SHADER);

	GLint Result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(ShaderProgram_id);
	glGetProgramiv(ShaderProgram_id, GL_LINK_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(ShaderProgram_id, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(ShaderProgram_id);
	glGetProgramiv(ShaderProgram_id, GL_VALIDATE_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(ShaderProgram_id, sizeof(eLog), nullptr, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// Bind XMoveOffset uniform variable
	UniformXMoveOffsetVar_id = glGetUniformLocation(ShaderProgram_id, "XMoveOffset");
	UniformModelMatrix_id = glGetUniformLocation(ShaderProgram_id, "ModelMatrix");
	UniformProjectionMatrix_id = glGetUniformLocation(ShaderProgram_id, "ProjectionMatrix");
}

void CreateTriangle()
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
	///* GLFW initialization *///
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Setup GLFW window properties
	GLFWwindow *MainWindow = glfwCreateWindow(WIDTH,
											HIGHT,
											"App window",
											nullptr,
											nullptr);

	if (!MainWindow)
	{
		printf ("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLFW to use (here is the main window)
	glfwMakeContextCurrent(MainWindow);

	///* GLEW initialization *///
	// Allow experimental (modern) extensions to use
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		printf ("GLEW initialization failed!");
		glfwDestroyWindow(MainWindow);
		glfwTerminate();

		return 1;
	}
	/// * END of GLEW initialization* ///

	glEnable(GL_DEPTH_TEST);
	
	// Setup viewport size. It is less then window size and equal to drawing area (FrameBuffer)
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 ProjectionMatrix = glm::perspective(45.0f, static_cast<GLfloat>(WIDTH) / static_cast<GLfloat>(HIGHT), 0.1f, 100.0f); // initialize projection matrix

	// Render loop
	while (!glfwWindowShouldClose(MainWindow))
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
		glUseProgram(ShaderProgram_id);

		// Scaling using uniform var for 'w'
		glUniform1f(UniformXMoveOffsetVar_id, ShapeOffset);
		// Set Projection matrix
		glUniformMatrix4fv(UniformProjectionMatrix_id, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

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
		glfwSwapBuffers(MainWindow);
	}

	return 0;
}