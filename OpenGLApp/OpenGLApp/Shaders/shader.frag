#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec4 VertColor;
in vec3 VertexPosition;

struct FDirectionalLight
{
	vec4 Color;

	float AmbientIntensity;
	float DiffuseIntensity;

	vec3 Direction;
};

struct FMaterial
{
	float SpecularIntensity;
	float Shininess;
};

uniform sampler2D Texture;
uniform FDirectionalLight SunLight;
uniform FMaterial Material;
uniform vec3 CameraPosition;

void main()
{
	vec4 AmbientColor = SunLight.Color * SunLight.AmbientIntensity;

	float DiffuseFactor = max(dot(normalize(Normal), normalize(SunLight.Direction)),  0.0f); // dot(A, B) = |A|*|B|*cos(angle), with normalized it is equal to == cos(angle)
	vec4 DiffuseColor = SunLight.Color * SunLight.DiffuseIntensity * DiffuseFactor;
	
	vec4 SpecularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if(DiffuseFactor > 0.0f)
	{
		vec3 VertexToCameraVector = normalize(CameraPosition - VertexPosition);
		vec3 ReflectionVector = normalize(-reflect(SunLight.Direction, normalize(Normal)));

		float SpecularFactor = max(dot(VertexToCameraVector, ReflectionVector), 0.0f);
		
		SpecularFactor = pow(SpecularFactor, Material.Shininess);
		SpecularColor = vec4(SunLight.Color * Material.SpecularIntensity * SpecularFactor);
	}

	FragColor = texture(Texture, TexCoord) * (AmbientColor + DiffuseColor + SpecularColor);

	float gamma = 1.2;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}