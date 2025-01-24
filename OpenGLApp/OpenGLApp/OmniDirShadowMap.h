#pragma once
#include "ShadowMap.h"
class OmniDirShadowMap : public ShadowMap
{
public: //TODO remove virtual calls here and into parent class
	virtual bool Init(GLint InitWidth, GLint InitHeight);
	virtual void Write();
	virtual void Read(GLenum TextureUnit) const;
};

