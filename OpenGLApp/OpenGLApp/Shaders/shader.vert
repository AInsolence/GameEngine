#version 330

layout (location = 0) in vec3 pos;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionMatrix;

out vec4 VertexColor;

void main()
{
	gl_Position = ProjectionMatrix * ModelMatrix * vec4(pos, 1.0);
	VertexColor = vec4(clamp(pos, 0.3f, 1.0f), 1.0f);
}