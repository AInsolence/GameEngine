#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec4 VertColor;

struct DirectionalLight
{
	vec4 Color;

	float AmbientIntensity;
	float DiffuseIntensity;

	vec3 Direction;
};

uniform sampler2D MyTexture;
uniform DirectionalLight SunLight;

void main()
{
	vec4 AmbientColor = SunLight.Color * SunLight.AmbientIntensity;

	float DiffuseFactor = max(dot(normalize(Normal), normalize(SunLight.Direction)),  0.0f); // dot(A, B) = |A|*|B|*cos(angle), with normalized it is equal to == cos(angle)
	
	vec4 DiffuseColor = SunLight.Color * SunLight.DiffuseIntensity * DiffuseFactor;

	FragColor = texture(MyTexture, TexCoord) * (AmbientColor + DiffuseColor);
}