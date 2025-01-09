#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	explicit Texture(const char* Path);
	~Texture();

	void LoadTexture();
	void Apply() const;
	void ClearTexture();

private:
	GLuint Id;

	int Width;
	int Height;
	int BitDepth;

	const char* FilePath;
};

