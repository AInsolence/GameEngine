#include "Components/Material.h"

Material::Material()
{
	SpecularIntensity = 0.0f;
	Shininess = 0.0f;
}

Material::Material(GLfloat InitSpecularIntensity, GLfloat InitShininess)
{
	SpecularIntensity = InitSpecularIntensity;
	Shininess = InitShininess;
}

void Material::Apply(GLint SpecularIntensityLocation, GLint ShininessLocation) const
{
	glUniform1f(SpecularIntensityLocation, SpecularIntensity);
	glUniform1f(ShininessLocation, Shininess);
}
