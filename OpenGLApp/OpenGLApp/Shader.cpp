#include "Shader.h"

#include <sstream>

Shader::Shader(const char* VertexShaderPath,
               const char* FragmentShaderPath)
{
	Id = 0;
	UniformModel = 0;
	UniformProjection = 0;

	CreateFromFile(VertexShaderPath, FragmentShaderPath);
}

Shader::~Shader()
{
	Clear();
}

void Shader::CreateFromFile(const char* VertexShaderPath, const char* FragmentShaderPath)
{
	const std::string VertexShaderCode = ReadFile(VertexShaderPath);
	const std::string FragmentShaderCode = ReadFile(FragmentShaderPath);

	CompileShaders(VertexShaderCode.c_str(),
					FragmentShaderCode.c_str());
}

GLint Shader::GetProjectionLocation() const
{
	return UniformProjection;
}

GLint Shader::GetViewLocation() const
{
	return UniformView;
}

GLint Shader::GetModelLocation() const
{
	return UniformModel;
}

GLint Shader::GetDirectionalLightColorLocation() const
{
	return UniformDirectionalLightColor;
}

GLint Shader::GetDirectionalLightIntensityLocation() const
{
	return UniformDirectionalLightIntensity;
}

void Shader::Use() const
{
	glUseProgram(Id);
}

void Shader::Clear()
{
	if (Id != 0)
	{
		glDeleteProgram(Id);
		Id = 0;
	}
}

GLuint Shader::GetId() const
{
	return Id;
}

void Shader::CompileShaders(const char* VertexShaderCode,
                            const char* FragmentShaderCode)
{
	Id = glCreateProgram();

	if (!Id)
	{
		printf("Error creating shader program");
		return;
	}

	AddShader(Id, VertexShaderCode, GL_VERTEX_SHADER);
	AddShader(Id, FragmentShaderCode, GL_FRAGMENT_SHADER);

	GLint Result = 0;
	GLchar ELog[1024] = {0};

	glLinkProgram(Id);
	glGetProgramiv(Id, GL_LINK_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(Id, sizeof(ELog), nullptr, ELog);
		printf("Error linking program: '%s'\n", ELog);
		return;
	}

	glValidateProgram(Id);
	glGetProgramiv(Id, GL_VALIDATE_STATUS, &Result);

	if (!Result)
	{
		glGetProgramInfoLog(Id, sizeof(ELog), nullptr, ELog);
		printf("Error validating program: '%s'\n", ELog);
		return;
	}

	// Setup uniform variables
	UniformProjection = glGetUniformLocation(Id, "ProjectionMatrix");
	UniformView = glGetUniformLocation(Id, "ViewMatrix");
	UniformModel = glGetUniformLocation(Id, "ModelMatrix");
	UniformDirectionalLightColor = glGetUniformLocation(Id, "SunLight.Color");
	UniformDirectionalLightIntensity = glGetUniformLocation(Id, "SunLight.Intensity");
}

void Shader::AddShader(GLuint ShaderProgramId, const char* ShaderCode, GLenum ShaderType) const
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
	GLchar ELog[1024] = {0};

	glGetShaderiv(LocalShader, GL_COMPILE_STATUS, &Result);

	if (!Result)
	{
		glGetShaderInfoLog(LocalShader, sizeof(ELog), nullptr, ELog);
		printf("Error compiling the %d shader: '%s'\n", ShaderType, ELog);
		return;
	}

	// Attach compiled shader to the shader program
	glAttachShader(ShaderProgramId, LocalShader);
}

std::string Shader::ReadFile(const char* FilePath)
{
	std::ifstream FileStream(FilePath, std::ios::in);

	if (!FileStream.is_open())
	{
		std::cerr << "Error: Unable to open file: " << FilePath << std::endl;
		return "";
	}

	std::stringstream Buffer;
	Buffer << FileStream.rdbuf();

	std::string Content = Buffer.str();

	FileStream.close();

	std::cout << "File Content (" << FilePath << "):\n\n" << Content << std::endl;

	return Content;
}
