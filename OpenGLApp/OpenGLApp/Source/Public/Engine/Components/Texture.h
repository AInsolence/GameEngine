#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	explicit Texture(const char* Path);
	explicit Texture(std::string&& Path);
	explicit Texture(const std::string& Path);

	~Texture();

	bool LoadTexture_RGB();
	bool LoadTexture_RGBA();
	bool LoadTextureFromMemory(unsigned char* data, unsigned int size);

	void Apply() const;
	void ClearTexture() const;

private:
	bool LoadTextureData(unsigned char* TextureData, unsigned int Channels = 4);

	GLuint Id;

	int Width;
	int Height;
	int BitDepth;

	std::string FilePath;
};

