#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec4 VertColor;
in vec3 FragmentPosition;

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

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

struct FSpotLight
{
	FPointLight PointLight;

	vec3 Direction;
	float CutOffAngleCos;
};

struct FMaterial
{
	float SpecularIntensity;
	float Shininess;
};

uniform FDirectionalLight SunLight;
uniform FPointLight PointLights [MAX_POINT_LIGHTS];
uniform FSpotLight SpotLights [MAX_SPOT_LIGHTS];
uniform int PointLightsCount;
uniform int SpotLightsCount;

uniform sampler2D Texture;
uniform FMaterial Material;

uniform vec3 CameraPosition;

vec4 CalculateLightByDirection(FBaseLight Base, vec3 Direction)
{
	vec4 AmbientColor = Base.Color * Base.AmbientIntensity;

	vec3 NormalizedDirection = normalize(Direction);

	float DiffuseFactor = max(dot(normalize(Normal), NormalizedDirection), 0.0f); // dot(A, B) = |A|*|B|*cos(angle), with normalized it is equal to == cos(angle)
	vec4 DiffuseColor = Base.Color * Base.DiffuseIntensity * DiffuseFactor;
	
	vec4 SpecularColor = vec4(0.0f );

	if(DiffuseFactor > 0.0f)
	{
		vec3 VertexToCameraVector = normalize(FragmentPosition - CameraPosition);
		vec3 ReflectionVector = normalize(reflect(NormalizedDirection, normalize(Normal)));

		float SpecularFactor = max(dot(VertexToCameraVector, ReflectionVector), 0.0f);
		
		SpecularFactor = pow(SpecularFactor, Material.Shininess);
		SpecularColor = vec4(Base.Color * Material.SpecularIntensity * SpecularFactor);
	}

	return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalculatePointLightColor(FPointLight PointLight)
{
	vec3 VectorToLight = PointLight.Position - FragmentPosition;
	float Distance = length(VectorToLight);
	vec3 Direction = normalize(VectorToLight);

	float InnerRadius = max(PointLight.InnerRadius, 0.0f);
	float OuterRadius = max(PointLight.OuterRadius, 0.0f);
	InnerRadius = min(InnerRadius, OuterRadius);
	OuterRadius = max(OuterRadius, InnerRadius + 0.001f);

	float Attenuation = max(PointLight.Exponent * Distance * Distance +
							PointLight.Linear * Distance +
							PointLight.Constant,
							0.0001f);

	if (InnerRadius == OuterRadius)
	{
		vec4 Color = CalculateLightByDirection(PointLight.Base, Direction);
		return (Color / Attenuation);
	}

	float ScaledSharpness = PointLight.RadiusSharpness / OuterRadius;
	float SmoothEdgeAttenuation = 1.0f / (1.0f + exp((Distance - OuterRadius) * ScaledSharpness));

	//float SmoothEdgeAttenuation = clamp(1.0 - (Distance - InnerRadius) / (OuterRadius - InnerRadius), 0.0, 1.0);

	vec4 Color = CalculateLightByDirection(PointLight.Base, Direction);

	return ((Color / Attenuation) * SmoothEdgeAttenuation);
}

vec4 CalculateSpotLightColor (FSpotLight SpotLight)
{
	vec3 Direction = normalize(FragmentPosition - SpotLight.PointLight.Position);

	float SpotLightFactor = dot(Direction, normalize(SpotLight.Direction));

	if (SpotLightFactor <= SpotLight.CutOffAngleCos)
	{
		return vec4(0.0f);
	}

	vec4 Color = CalculatePointLightColor(SpotLight.PointLight);

	return Color * (1.0f - (1.0f - SpotLightFactor) * (1.0f / (1.0f - SpotLight.CutOffAngleCos)));
}

vec4 CalculateAllPointLightsColor()
{
	vec4 PointLightColor = vec4(0.0f);

	int LightsToProcess = min(PointLightsCount, MAX_POINT_LIGHTS);

	for (int PointLightIndex = 0; PointLightIndex < LightsToProcess; PointLightIndex++)
	{
		PointLightColor += CalculatePointLightColor(PointLights[PointLightIndex]);
	}

	return PointLightColor;
}

vec4 CalculateAllSpotLightsColor()
{
	vec4 SpotLightColor = vec4(0.0f);

	int LightsToProcess = min(SpotLightsCount, MAX_SPOT_LIGHTS);

	for (int SpotLightIndex = 0; SpotLightIndex < LightsToProcess; SpotLightIndex++)
	{
		SpotLightColor += CalculateSpotLightColor(SpotLights[SpotLightIndex]);
	}

	return SpotLightColor;
}

void main()
{
	vec4 DirectionalLightColor = CalculateLightByDirection(SunLight.Base, normalize(SunLight.Direction));
	vec4 PointLightColor = CalculateAllPointLightsColor();
	vec4 SpotLightColor = CalculateAllSpotLightsColor();

	FragColor = texture(Texture, TexCoord) * (DirectionalLightColor + PointLightColor + SpotLightColor);
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // normals debug line

	float gamma = 1.0;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}