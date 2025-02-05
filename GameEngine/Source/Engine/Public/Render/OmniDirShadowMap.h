#pragma once
#include "ShadowMap.h"
class OmniDirShadowMap : public ShadowMap
{
public:
	virtual bool Init(GLint InitWidth, GLint InitHeight);
	virtual void Write();
	virtual void Read(GLenum TextureUnit) const;
};

