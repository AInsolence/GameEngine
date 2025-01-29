#version 330

in vec4 FragmentPosition;

uniform vec3 OmniLightPosition;
uniform float FarPlane;

void main()
{
	float Distance = length(FragmentPosition.xyz - OmniLightPosition);
	Distance = clamp(Distance / FarPlane, 0.0, 1.0);
	gl_FragDepth = Distance;
}