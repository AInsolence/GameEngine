#include "StaticHelper.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

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