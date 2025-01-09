#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 VertTexture;

out vec2 TexCoord;
out vec4 VertColor;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
	VertColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	TexCoord = VertTexture;
}