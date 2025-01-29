#version 330

layout (location = 0) in vec3 VertPosition;

uniform mat4 ModelMatrix;
uniform mat4 DirectionalLightSpaceTransform;

void main()
{
	gl_Position = DirectionalLightSpaceTransform * ModelMatrix * vec4(VertPosition, 1.0f);
}