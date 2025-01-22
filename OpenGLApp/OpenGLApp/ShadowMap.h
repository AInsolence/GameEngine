#pragma once

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();
	virtual ~ShadowMap();

	virtual bool Init(GLint InitWidth, GLint InitHeight);
	virtual void Write();
	virtual void Read(GLenum TextureUnit);

	GLint GetShadowWidth() const { return ShadowWidth; }
	GLint GetShadowHeight() const { return ShadowHeight; }

protected:

	void Clear() const;
	GLuint FBO, Id;
	GLint ShadowWidth, ShadowHeight;
};

