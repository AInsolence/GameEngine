#include "Components/Skybox.h"

#include "glm.hpp"
#include <gtc/type_ptr.hpp>

#include "Render/Shader.h"
#include "Components/Mesh.h"

#include "Utilities/Helper.h"
#include "Utilities/stb_image.h"

Skybox::Skybox(std::array<std::string, 6> FaceLocations)
{
	SkyShader = std::make_unique<Shader>("Source/Engine/Public/Render/Shaders/skybox_shader.vert",
											"Source/Engine/Public/Render/Shaders/skybox_shader.frag");

	UniformProjectionMatrix = SkyShader->GetProjectionLocation();
	UniformViewMatrix = SkyShader->GetViewLocation();

	// Set sky textures
	glGenTextures(1, &TextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

	int Width, Height, BitDepth;

	
	for (int Face = 0; Face < 6; ++Face)
	{
		unsigned char* TextureData = stbi_load(FaceLocations[Face].data(), 
													&Width, 
													&Height, 
													&BitDepth, 
													STBI_rgb);

		if (!TextureData)
		{
			printf("Failed to load RGB texture from path: %s (%s, %i)\n ",
										FaceLocations[Face].c_str(), __FILE__, __LINE__);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face, 0,
						GL_RGB, Width, Height, 
						0, GL_RGB, 
						GL_UNSIGNED_BYTE, TextureData);

		stbi_image_free(TextureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh
	SkyMesh = std::make_unique<Mesh>(SkyboxVertices, SkyboxIndices);
}

void Skybox::DrawSky(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
	ViewMatrix = glm::mat4(glm::mat3(ViewMatrix));

	glDepthMask(GL_FALSE);

	SkyShader->Use();

	// Set Projection matrix
	glUniformMatrix4fv(UniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	// Set view matrix
	glUniformMatrix4fv(UniformViewMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

	glActiveTexture(GL_TEXTURE1);
	Helper::EnsureGL("glActiveTexture", __FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

	SkyShader->SetSkyboxTextureUnit(1);

	SkyShader->Validate();
	SkyMesh->Render();

	glDepthMask(GL_TRUE);
}
