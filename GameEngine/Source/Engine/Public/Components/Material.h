#pragma once
#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat InitSpecularIntensity, GLfloat InitShininess);

	void Apply(GLint SpecularIntensityLocation, GLint ShininessLocation) const;

protected:
	GLfloat SpecularIntensity;
	GLfloat Shininess;
};

