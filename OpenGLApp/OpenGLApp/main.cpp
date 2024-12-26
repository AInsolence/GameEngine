#include <cmath>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GL window dimension
const GLint WIDTH = 800;
const GLint HIGHT = 600;

// Define main variables

GLuint VAOid;
GLuint VBOid;
GLuint ShaderProgram_id;
GLuint UniformXMoveOffsetVar_id;

bool Direction = true;
float ShapeOffset = 0.0f;
float ShapeMaxOffset = 0.6f;
float ShapeOffsetStep = 0.005f;

// Vertex Shader code
static const char* VertexShaderCode = R"gl(

	#version 330

	layout (location = 0) in vec3 pos;

	uniform float XMoveOffset;

	void main()
	{
		gl_Position = vec4(0.4 * pos.x - XMoveOffset, 0.4 * pos.y, pos.z, 3.0  + XMoveOffset * 3);
	}

)gl";

// Fragment Shader code
static const char* FragmentShaderCode = R"gl(

	#version 330

	out vec4 color;

	void main()
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
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
}

void CreateTriangle()
{
	// Define triangle's vertices
	constexpr GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0,  0.0f
	};

	glGenVertexArrays(1, &VAOid); // first param here number of VAOs we are creating
	glBindVertexArray(VAOid);

	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// With attributes explaining how to use data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	
	// Setup viewport size. It is less then window size and equal to drawing area (FrameBuffer)
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Render loop
	while (!glfwWindowShouldClose(MainWindow))
	{
		// Get and handle user input events
		glfwPollEvents();

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

		// Clear window with black color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		///* Draw triangle *///
		glUseProgram(ShaderProgram_id);

		glUniform1f(UniformXMoveOffsetVar_id, ShapeOffset);

		glBindVertexArray(VAOid);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);
		///* END of draw triangle *///
		glfwSwapBuffers(MainWindow);
	}

	return 0;
}