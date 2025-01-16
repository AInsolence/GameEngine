#version 330

layout (location = 0) in vec3 VertPosition;
layout (location = 1) in vec2 VertTexture;
layout (location = 2) in vec3 VertNormal;

out vec2 TexCoord;
out vec4 Color;
out vec3 Normal;
out vec3 FragmentPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertPosition, 1.0);
	
	Color = vec4(clamp(VertPosition, 0.0f, 1.0f), 1.0f);

	TexCoord = VertTexture;

	Normal = mat3(transpose(inverse(ModelMatrix))) * VertNormal;

	FragmentPosition = (ModelMatrix * vec4(VertPosition, 1.0)).xyz;
}