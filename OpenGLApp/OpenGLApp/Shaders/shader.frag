#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec4 VertColor;
in vec3 FragmentPosition;

const int MAX_POINT_LIGHTS = 10;

struct FBaseLight
{
	vec4 Color;

	float AmbientIntensity;
	float DiffuseIntensity;
};

struct FDirectionalLight
{
	FBaseLight Base;

	vec3 Direction;
};

struct FPointLight
{
	FBaseLight Base;

	vec3 Position;
	float Exponent;
	float Linear;
	float Constant;

	float InnerRadius;
	float OuterRadius;
	float RadiusSharpness;
};

struct FMaterial
{
	float SpecularIntensity;
	float Shininess;
};

uniform FDirectionalLight SunLight;
uniform FPointLight PointLights [MAX_POINT_LIGHTS];
uniform int PointLightsCount;

uniform sampler2D Texture;
uniform FMaterial Material;

uniform vec3 CameraPosition;

vec4 CalculateLightByDirection(FBaseLight Base, vec3 Direction)
{
	vec4 AmbientColor = Base.Color * Base.AmbientIntensity;

	float DiffuseFactor = max(dot(normalize(Normal), normalize(Direction)),  0.0f); // dot(A, B) = |A|*|B|*cos(angle), with normalized it is equal to == cos(angle)
	vec4 DiffuseColor = Base.Color * Base.DiffuseIntensity * DiffuseFactor;
	
	vec4 SpecularColor = vec4(0.0f );

	if(DiffuseFactor > 0.0f)
	{
		vec3 VertexToCameraVector = normalize(CameraPosition - FragmentPosition);
		vec3 ReflectionVector = normalize(reflect(Direction, normalize(Normal)));

		float SpecularFactor = max(dot(VertexToCameraVector, ReflectionVector), 0.0f);
		
		SpecularFactor = pow(SpecularFactor, Material.Shininess);
		SpecularColor = vec4(Base.Color * Material.SpecularIntensity * SpecularFactor);
	}

	return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalculatePointLightColor()
{
	vec4 PointLightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int PointLightIndex = 0; PointLightIndex < PointLightsCount; PointLightIndex++)
	{
		vec3 Direction = FragmentPosition - PointLights[PointLightIndex].Position;
		float Distance = length(Direction);
		Direction = normalize(Direction);

		float InnerRadius = max(PointLights[PointLightIndex].InnerRadius, 0.0f);
		float OuterRadius = max(PointLights[PointLightIndex].OuterRadius, 0.0f);
		// check Inner radius always less then Outer
		InnerRadius = min(InnerRadius, OuterRadius);

		
		float Attenuation = PointLights[PointLightIndex].Exponent * Distance * Distance +
							PointLights[PointLightIndex].Linear * Distance +
							PointLights[PointLightIndex].Constant;

		//if (InnerRadius == OuterRadius)
		//{
			//vec4 Color = CalculateLightByDirection(PointLights[PointLightIndex].Base, Direction);
			//PointLightColor += (Color / Attenuation);
			//continue;
		//}

		float SmoothEdgeAttenuation = 1.0f / (1.0f + exp((Distance - OuterRadius) * PointLights[PointLightIndex].RadiusSharpness));
		//float SmoothEdgeAttenuation = clamp(1.0 - (Distance - InnerRadius) / (OuterRadius - InnerRadius), 0.0, 1.0);

		vec4 Color = CalculateLightByDirection(PointLights[PointLightIndex].Base, Direction);

		PointLightColor += ((Color / Attenuation) * SmoothEdgeAttenuation);
	}

	return PointLightColor;
}

void main()
{
	vec4 DirectionalLightColor = CalculateLightByDirection(SunLight.Base, SunLight.Direction);
	vec4 PointLightColor = CalculatePointLightColor();

	FragColor = texture(Texture, TexCoord) * (DirectionalLightColor + PointLightColor);
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
	float gamma = 1.0;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}