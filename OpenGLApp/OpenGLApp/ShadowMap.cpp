#include "ShadowMap.h"

#include <cstdio>

ShadowMap::ShadowMap()
{
	FBO = 0;
	Id = 0;
}

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (Id)
	{
		glDeleteTextures(1, &Id);
	}
}

bool ShadowMap::Init(GLint InitWidth, GLint InitHeight)
{
	ShadowWidth = InitWidth;
	ShadowHeight = InitHeight;

	glGenBuffers(1, &FBO);
	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Id, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	const GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i/n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, Id);
}
