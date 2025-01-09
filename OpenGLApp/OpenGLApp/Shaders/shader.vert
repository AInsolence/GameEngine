#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 VertTexture;

out vec2 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
	TexCoord = VertTexture;
}