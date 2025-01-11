#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertColor;

struct DirectionalLight
{
	vec4 Color;
	float Intensity;
};

uniform sampler2D MyTexture;
uniform DirectionalLight SunLight;

void main()
{
	vec4 SunLightColor = SunLight.Color * SunLight.Intensity;

	FragColor = texture(MyTexture, TexCoord) * SunLightColor;
}