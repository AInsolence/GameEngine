#include "ShadowMap.h"

#include <cstdio>

#include "StaticHelper.h"

ShadowMap::ShadowMap()
{
	FBO = 0;
	Id = 0;
}

ShadowMap::~ShadowMap()
{
	Clear();
}

bool ShadowMap::Init(GLint InitWidth, GLint InitHeight)
{
	Clear();

	ShadowWidth = InitWidth;
	ShadowHeight = InitHeight;

	glGenFramebuffers(1, &FBO);
	ENSURE_GL("glGenFramebuffers");

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);
	ENSURE_GL("glBindTexture");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	ENSURE_GL("glTexImage2D");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	ENSURE_GL("glBindFramebuffer");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Id, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	const GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("[ShadowMap.cpp] Framebuffer error: %i, (%s, %d)\n", FramebufferStatus, __FILE__, __LINE__);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // should be unbind
	ENSURE_GL("glBindFramebuffer");

	const GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete: %x, (%s, %d)\n", FramebufferStatus, __FILE__, __LINE__);
	}
}

void ShadowMap::Read(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, Id);
	ENSURE_GL("glBindTexture");
}

void ShadowMap::Clear() const
{
	if (FBO != 0)
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (Id != 0)
	{
		glDeleteTextures(1, &Id);
	}
}
