#version 330

layout (location = 0) in vec3 VertPosition;

uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ModelMatrix * vec4(VertPosition, 1.0);
}