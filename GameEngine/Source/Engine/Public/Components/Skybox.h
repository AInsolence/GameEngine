#pragma once

#include <array>
#include <fwd.hpp>
#include <memory>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "Components/Mesh.h"

class Shader;

class Skybox
{
public:
	Skybox(std::array<std::string, 6> FaceLocations);

	void DrawSky(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);

private:
	std::unique_ptr<Mesh> SkyMesh;
	std::unique_ptr<Shader> SkyShader;

	GLuint TextureId;
	GLint UniformProjectionMatrix;
	GLint UniformViewMatrix;

	std::vector<unsigned int> SkyboxIndices = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	std::vector<GLfloat> SkyboxVertices = {
	//	   x      y      z     u     v     nx     ny    nz
		-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};
};

