#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"

#include <ostream>

#include "StaticHelper.h"

Texture::Texture(const char* Path)
{
	Id = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FilePath = Path;
}

Texture::Texture(std::string&& Path)
{
	Id = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FilePath = std::move(Path);
}

Texture::Texture(const std::string& Path)
{
	Id = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FilePath = Path;
}

Texture::~Texture()
{
	ClearTexture();
}

bool Texture::LoadTexture_RGB()
{
	unsigned char* TextureData = stbi_load(FilePath.data(), &Width, &Height, &BitDepth, STBI_rgb_alpha);

	if (!TextureData)
	{
		printf("Failed to load RGB texture from path: %s\n ", FilePath.c_str());
		return false;
	}

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unsigned char dummyData[4] = { 255, 0, 0, 255 }; // Red pixel
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummyData); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
	StaticHelper::EnsureGLFunction("glTexImage2D");
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);

	return true;
}

bool Texture::LoadTexture_RGBA()
{
	unsigned char* TextureData = stbi_load(FilePath.data(), &Width, &Height, &BitDepth, STBI_rgb_alpha);

	if (!TextureData)
	{
		printf("Failed to load RGBA texture from path: %s\n ", FilePath.c_str());
		return false;
	}

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//unsigned char dummyData[4] = { 255, 0, 0, 255 }; // Red pixel
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummyData); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
	StaticHelper::EnsureGLFunction("glTexImage2D");
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);

	return true;
}

void Texture::Apply() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Id);

	StaticHelper::EnsureGLFunction("glBindTexture");
}

void Texture::ClearTexture() const
{
	glDeleteTextures(1, &Id);
}

