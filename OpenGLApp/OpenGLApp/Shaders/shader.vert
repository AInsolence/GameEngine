#version 330

layout (location = 0) in vec3 pos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec4 VertexColor;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(pos, 1.0);
	VertexColor = vec4(clamp(pos, 0.3f, 1.0f), 1.0f);
}