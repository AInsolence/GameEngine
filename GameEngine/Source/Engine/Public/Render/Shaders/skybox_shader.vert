#version 330

layout (location = 0) in vec3 VertPosition;

out vec3 TextureCoordinates;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	TextureCoordinates = VertPosition;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(VertPosition, 1.0);
}