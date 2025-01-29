#include "Render/SpotLight.h"

#include <gtc/type_ptr.hpp>

SpotLight::SpotLight(glm::vec4 InitColor,
					GLfloat InitAmbientIntensity,
					GLfloat InitDiffuseIntensity,
					glm::vec3 InitPosition,
					glm::vec3 InitDirection,
					GLfloat InitCutOffAngle,
					GLfloat InitExponent,
					GLfloat InitLinear,
					GLfloat InitConstant,
					GLfloat InitInnerRadius,
					GLfloat InitOuterRadius,
					GLfloat InitRadiusSharpness,
					GLint InitShadowWidth,
					GLint InitShadowHeight,
					GLfloat InitNear,
					GLfloat InitFar) : PointLight(InitColor,
														InitAmbientIntensity,
														InitDiffuseIntensity,
														InitPosition,
														InitExponent,
														InitLinear,
														InitConstant,
														InitInnerRadius,
														InitOuterRadius,
														InitRadiusSharpness,
														InitShadowWidth,
														InitShadowHeight,
														InitNear,
														InitFar)
{
	Direction = InitDirection;
	CutOffAngleCos = cosf(glm::radians(InitCutOffAngle));
}

void SpotLight::Apply(GLint ColorLocation,
						GLint AmbientIntensityLocation,
						GLint DiffuseIntensityLocation,
						GLint PositionLocation,
						GLint DirectionLocation,
						GLint CutOffAngleCosLocation,
						GLint ExponentLocation,
						GLint LinearLocation,
						GLint ConstantLocation,
						GLint InnerRadiusLocation,
						GLint OuterRadiusLocation,
						GLint RadiusSharpnessLocation) const
{
	glUniform4fv(ColorLocation, 1, glm::value_ptr(Color));

	if (isOn)
	{
		glUniform1f(AmbientIntensityLocation, AmbientIntensity);
		glUniform1f(DiffuseIntensityLocation, DiffuseIntensity);
	}
	else
	{
		glUniform1f(AmbientIntensityLocation, 0.0f);
		glUniform1f(DiffuseIntensityLocation, 0.0f);
	}


	glUniform3fv(PositionLocation, 1, glm::value_ptr(Position));
	glUniform3fv(DirectionLocation, 1, glm::value_ptr(Direction));

	glUniform1f(CutOffAngleCosLocation, CutOffAngleCos);

	glUniform1f(ExponentLocation, Exponent);
	glUniform1f(LinearLocation, Linear);
	glUniform1f(ConstantLocation, Constant);

	glUniform1f(InnerRadiusLocation, InnerRadius);
	glUniform1f(OuterRadiusLocation, OuterRadius);
	glUniform1f(RadiusSharpnessLocation, RadiusSharpness);
}

void SpotLight::SetTransform(glm::vec3 InPosition, glm::vec3 InDirection)
{
	Position = InPosition;
	Direction = InDirection;
}

void SpotLight::Toggle()
{
	isOn = !isOn;
}
