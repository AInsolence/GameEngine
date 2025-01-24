#version 330

in vec4 FragPosition;

uniform vec3 OmniLightPosition;
uniform float FarPlane;

void main()
{
	float Distance = length(FragPosition.xyz - OmniLightPosition);
	Distance = Distance / FarPlane;
	gl_FragDepth = Distance;
}