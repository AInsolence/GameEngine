#include "Texture.h"

Texture::Texture(const char* Path)
{
	Id = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FilePath = Path;

	LoadTexture();
}

Texture::~Texture()
{
	ClearTexture();
}

void Texture::LoadTexture()
{
	unsigned char* TextureData = stbi_load(FilePath, &Width, &Height, &BitDepth, STBI_rgb_alpha);

	if (!TextureData)
	{
		printf("Failed to load texture from path: %s\n ", FilePath);
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
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(TextureData);
}

void Texture::Apply() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Id);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &Id);
	Id = 0;
	Width = 0;
	Height = 0;
	BitDepth = 0;
	FilePath = "";
}

