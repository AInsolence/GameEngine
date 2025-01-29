#include "Render/OmniDirShadowMap.h"

#include "Utilities/Helper.h"

bool OmniDirShadowMap::Init(GLint InitWidth, GLint InitHeight)
{
	Clear();

	ShadowWidth = InitWidth;
	ShadowHeight = InitHeight;

	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
						ShadowWidth, ShadowHeight, 0,
						GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	Helper::EnsureGL("glBindFramebuffer", __FILE__, __LINE__);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Id, 0);

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

void OmniDirShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // should be unbind

	Helper::EnsureGL("glBindFramebuffer", __FILE__, __LINE__);

	const GLenum FBStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBStatus != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete: %x, (%s, %d)\n", FBStatus, __FILE__, __LINE__);
	}
}

void OmniDirShadowMap::Read(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
	Helper::EnsureGL("glBindTexture", __FILE__, __LINE__);
}