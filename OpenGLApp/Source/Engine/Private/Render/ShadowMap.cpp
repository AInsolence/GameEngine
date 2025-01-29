#include "Render/ShadowMap.h"

#include <cstdio>

#include "Utilities/Helper.h"

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
	Helper::EnsureGL("glGenFramebuffers", __FILE__, __LINE__);

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_2D, Id);
	Helper::EnsureGL("glBindTexture", __FILE__, __LINE__);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	Helper::EnsureGL("glTexImage2D", __FILE__, __LINE__);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	Helper::EnsureGL("glBindFramebuffer", __FILE__, __LINE__);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Id, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	const GLenum FBStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("[ShadowMap.cpp] Framebuffer error: %i, (%s, %d)\n", FBStatus, __FILE__, __LINE__);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // should be unbind
	Helper::EnsureGL("glBindFramebuffer", __FILE__, __LINE__);

	const GLenum FBStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete: %x, (%s, %d)\n", FBStatus, __FILE__, __LINE__);
	}
}

void ShadowMap::Read(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, Id);
	Helper::EnsureGL("glBindTexture", __FILE__, __LINE__);
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
