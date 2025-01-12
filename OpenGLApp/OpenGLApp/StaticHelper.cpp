#include "StaticHelper.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <geometric.hpp>
#include <vec3.hpp>

std::vector<GLenum> StaticHelper::EnsureGLFunction(const char* FunctionName)
{
#if defined (DEBUG) || defined (_DEBUG)
	GLenum Error;
	std::vector<GLenum> Errors;
	std::ostringstream ErrorMessages;

	std::ofstream LogFile("opengl_errors.log", std::ios::app);

	time_t timestamp;
	const auto CurrentTime = time(&timestamp);

	while ((Error = glGetError()) != GL_NO_ERROR)
	{
		Errors.push_back(Error);

		char TimeStr[26];
		ctime_s(TimeStr, sizeof(TimeStr), &CurrentTime);
		ErrorMessages << "[" << TimeStr << "] OpenGL error in " << FunctionName << ": ";

		switch (Error)
		{
			case GL_INVALID_ENUM:
				ErrorMessages << "GL_INVALID_ENUM - An unacceptable value is specified for an enumerated argument.";
				break;
			case GL_INVALID_VALUE:
				ErrorMessages << "GL_INVALID_VALUE - A numeric argument is out of range.";
				break;
			case GL_INVALID_OPERATION:
				ErrorMessages << "GL_INVALID_OPERATION - The specified operation is not allowed in the current state.";
				break;
			case GL_STACK_OVERFLOW:
				ErrorMessages << "GL_STACK_OVERFLOW - This command would cause a stack overflow.";
				break;
			case GL_STACK_UNDERFLOW:
				ErrorMessages << "GL_STACK_UNDERFLOW - This command would cause a stack underflow.";
				break;
			case GL_OUT_OF_MEMORY:
				ErrorMessages << "GL_OUT_OF_MEMORY - There is not enough memory left to execute the command.";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				ErrorMessages << "GL_INVALID_FRAMEBUFFER_OPERATION - The framebuffer object is not complete.";
				break;
			default:
				ErrorMessages << "Unknown error code: " << Error;
				break;
		}

		ErrorMessages << std::endl;
	}

	if (LogFile.is_open())
	{
		LogFile.close();
	}


	if (!Errors.empty())
	{
		std::cerr << ErrorMessages.str();
	}

	return Errors;

#else
	return {};
#endif
}

void StaticHelper::CalculateAverageNormals(unsigned int* Indices,
											unsigned int IndiceCount,
											GLfloat* Vertices,
											unsigned int VerticesCount,
											unsigned int VerticesOffset,
											unsigned int NormalsOffset)
{
	std::vector<glm::vec3> VertexNormals(VerticesCount / VerticesOffset, glm::vec3(0.0f));

	for (size_t i = 0; i < IndiceCount; i += 3)
	{
		const auto TriangleVert0 = Indices[i] * VerticesOffset;
		const auto TriangleVert1 = Indices[i + 1] * VerticesOffset;
		const auto TriangleVert2 = Indices[i + 2] * VerticesOffset;

		glm::vec3 v1(Vertices[TriangleVert1] - Vertices[TriangleVert0],
					Vertices[TriangleVert1 + 1] - Vertices[TriangleVert0 + 1],
					Vertices[TriangleVert1 + 2] - Vertices[TriangleVert0 + 2]);

		glm::vec3 v2(Vertices[TriangleVert2] - Vertices[TriangleVert0],
					Vertices[TriangleVert2 + 1] - Vertices[TriangleVert0 + 1],
					Vertices[TriangleVert2 + 2] - Vertices[TriangleVert0 + 2]);

		const glm::vec3 Normal = glm::normalize(glm::cross(v1, v2));

		VertexNormals[Indices[i]] += Normal;
		VertexNormals[Indices[i + 1]] += Normal;
		VertexNormals[Indices[i + 2]] += Normal;
	}

	for (size_t i = 0; i < VertexNormals.size(); i++)
	{
		const unsigned int Offset = i * VerticesOffset + NormalsOffset;
		const glm::vec3 normalizedNormal = glm::normalize(VertexNormals[i]);

		Vertices[Offset] = normalizedNormal.x;
		Vertices[Offset + 1] = normalizedNormal.y;
		Vertices[Offset + 2] = normalizedNormal.z;
	}
}
