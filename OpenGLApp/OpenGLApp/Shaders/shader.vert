#version 330

layout (location = 0) in vec3 VertPosition;
layout (location = 1) in vec2 VertTexture;
layout (location = 2) in vec3 VertNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragmentPosition;
out vec4 DirectionalLightSpacePosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 DirectionalLightSpaceTransform;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(VertPosition, 1.0);

	TexCoord = VertTexture;

	Normal = (mat3(transpose(inverse(ModelMatrix))) * VertNormal);

	FragmentPosition = (ModelMatrix * vec4(VertPosition, 1.0)).xyz;

	DirectionalLightSpacePosition = DirectionalLightSpaceTransform * ModelMatrix * vec4(VertPosition, 1.0f);
}