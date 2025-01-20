#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	explicit Texture(const char* Path);
	explicit Texture(std::string&& Path);
	explicit Texture(const std::string& Path);

	~Texture();

	bool LoadTexture_RGB();
	bool LoadTexture_RGBA();
	void Apply() const;
	void ClearTexture() const;

private:
	GLuint Id;

	int Width;
	int Height;
	int BitDepth;

	std::string FilePath;
};

