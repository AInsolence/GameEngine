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
	StaticHelper::EnsureGLFunction("glGenFramebuffers");

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	StaticHelper::EnsureGLFunction("glBindFramebuffer");
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Id, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	const GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{// should be unbind
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	StaticHelper::EnsureGLFunction("glBindFramebuffer");

	const GLenum FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete: %x\n", FramebufferStatus);
	}
}

void ShadowMap::Read(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, Id);
	StaticHelper::EnsureGLFunction("glBindTexture");
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
