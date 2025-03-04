#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal; 
in vec4 VertColor;
in vec3 FragmentPosition;
in vec4 DirectionalLightSpacePosition;

const vec3 PoissonDisk[16] = vec3[]
(
	vec3(0.355, 0.355, 0.355), vec3(-0.355, -0.355, 0.355),
	vec3(-0.355, 0.355, -0.355), vec3(0.355, -0.355, -0.355),
	vec3(0.707, 0.000, 0.707), vec3(-0.707, 0.000, 0.707),
	vec3(0.000, 0.707, -0.707), vec3(0.000, -0.707, -0.707),
	vec3(0.500, 0.866, 0.000), vec3(-0.500, -0.866, 0.000),
	vec3(0.866, -0.500, 0.000), vec3(-0.866, 0.500, 0.000),
	vec3(0.000, 0.000, 1.000), vec3(0.000, 0.000, -1.000),
	vec3(0.000, 1.000, 0.000), vec3(0.000, -1.000, 0.000)
);

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

struct FOmniShadowMap
{
	samplerCube ShadowMap;
	float FarPlane;
};

uniform FDirectionalLight SunLight;

uniform FPointLight PointLights [MAX_POINT_LIGHTS];
uniform FSpotLight SpotLights [MAX_SPOT_LIGHTS];
uniform int PointLightsCount;
uniform int SpotLightsCount;

uniform sampler2D Texture;
uniform sampler2D DirectionalShadowMap;
uniform FMaterial Material;

uniform FOmniShadowMap OmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform vec3 CameraPosition;

float CalculateDirLightShadowFactor(FDirectionalLight DirectionalLight)
{
	vec3 ProjCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
	ProjCoords = (ProjCoords * 0.5f) + 0.5f; // translate from (-1, 1) to (0, 1)

	float CurrentDepth = ProjCoords.z;

	if(CurrentDepth > 1.0f)
	{ // no shadows
		return 0.0f;
	}

	float Shadow = 0.0;

	vec3 NormalizedNormal = normalize(Normal);
	vec3 NormalizedDirection = normalize(DirectionalLight.Direction);
	float Bias = max(0.0005 * (1 - dot(NormalizedNormal, NormalizedDirection)), 0.0005);

	vec2 TexelSize = 1.0 / textureSize(DirectionalShadowMap, 0);

	for(int x = -3; x <= 3; ++x)
	{
		for(int y = -3; y <= 3; ++y)
		{
			float PCFdepth = texture(DirectionalShadowMap, ProjCoords.xy + vec2(x, y) * TexelSize).r;
			Shadow += CurrentDepth - Bias > PCFdepth ? 1.0f : 0.0f;
		}
	}

	Shadow /= 49.0f;

	return Shadow;
}

float CalculateOmniShadowFactor(FPointLight Light, int ShadowIndex)
{
	vec3 VectorToLight = FragmentPosition - Light.Position;	
	float Current = length(VectorToLight);

	float ShadowFactor = 0.0f;
	float Bias = 0.05;
	int Samples = 16;

	float DiskRadius = 0.01;

	for (int i = 0; i < Samples; i++)
	{
		vec3 SamplePosition = normalize(VectorToLight + PoissonDisk[i] * DiskRadius);
		float Closest = texture(OmniShadowMaps[ShadowIndex].ShadowMap, SamplePosition).r;
		Closest *= OmniShadowMaps[ShadowIndex].FarPlane;

		if (Current - Bias > Closest)
		{
			ShadowFactor += 1.0f;
		}
	}

	ShadowFactor /= float(Samples);

	return ShadowFactor;
}

vec4 CalculateLightByDirection(FBaseLight Base, vec3 Direction, float ShadowFactor)
{
	vec4 AmbientColor = Base.Color * Base.AmbientIntensity;

	vec3 NormalizedDirection = normalize(Direction);

	float DiffuseFactor = max(dot(normalize(Normal), NormalizedDirection), 0.0f); // dot(A, B) = |A|*|B|*cos(angle), with normalized it is equal to == cos(angle)
	vec4 DiffuseColor = Base.Color * Base.DiffuseIntensity * DiffuseFactor;
	
	vec4 SpecularColor = vec4(0.0f);

	if(DiffuseFactor > 0.0f)
	{
		vec3 VertexToCameraVector = normalize(FragmentPosition - CameraPosition);
		vec3 ReflectionVector = normalize(reflect(NormalizedDirection, normalize(Normal)));

		float SpecularFactor = max(dot(VertexToCameraVector, ReflectionVector), 0.0f);
		
		SpecularFactor = pow(SpecularFactor, Material.Shininess);
		SpecularColor = vec4(Base.Color * Material.SpecularIntensity * SpecularFactor);
	}

	return (AmbientColor + (1.0f - ShadowFactor) * (DiffuseColor + SpecularColor));
}

vec4 CalculatePointLightColor(FPointLight PointLight, int ShadowIndex)
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
	
	float ShadowFactor = CalculateOmniShadowFactor(PointLight, ShadowIndex);

	if (InnerRadius == OuterRadius)
	{
		vec4 Color = CalculateLightByDirection(PointLight.Base, Direction, ShadowFactor);
		return (Color / Attenuation);
	}

	float ScaledSharpness = PointLight.RadiusSharpness / OuterRadius;
	float SmoothEdgeAttenuation = 1.0f / (1.0f + exp((Distance - OuterRadius) * ScaledSharpness));

	//float SmoothEdgeAttenuation = clamp(1.0 - (Distance - InnerRadius) / (OuterRadius - InnerRadius), 0.0, 1.0);

	vec4 Color = CalculateLightByDirection(PointLight.Base, Direction, ShadowFactor);

	return ((Color / Attenuation) * SmoothEdgeAttenuation);
}

vec4 CalculateSpotLightColor (FSpotLight SpotLight, int ShadowIndex)
{
	vec3 Direction = normalize(FragmentPosition - SpotLight.PointLight.Position);

	float SpotLightFactor = dot(Direction, normalize(SpotLight.Direction));

	if (SpotLightFactor <= SpotLight.CutOffAngleCos)
	{
		return vec4(0.0f);
	}

	vec4 Color = CalculatePointLightColor(SpotLight.PointLight, ShadowIndex);

	return Color * (1.0f - (1.0f - SpotLightFactor) * (1.0f / (1.0f - SpotLight.CutOffAngleCos)));
}

vec4 CalculateAllPointLightsColor()
{
	vec4 PointLightColor = vec4(0.0f);

	int LightsToProcess = min(PointLightsCount, MAX_POINT_LIGHTS);

	for (int PointLightIndex = 0; PointLightIndex < LightsToProcess; PointLightIndex++)
	{
		PointLightColor += CalculatePointLightColor(PointLights[PointLightIndex], PointLightIndex);
	}

	return PointLightColor;
}

vec4 CalculateAllSpotLightsColor()
{
	vec4 SpotLightColor = vec4(0.0f);

	int LightsToProcess = min(SpotLightsCount, MAX_SPOT_LIGHTS);

	for (int SpotLightIndex = 0; SpotLightIndex < LightsToProcess; SpotLightIndex++)
	{
		SpotLightColor += CalculateSpotLightColor(SpotLights[SpotLightIndex], SpotLightIndex + PointLightsCount);
	}

	return SpotLightColor;
}

vec4 CalculateDirectionalLight()
{
	float ShadowFactor = CalculateDirLightShadowFactor(SunLight);
	return CalculateLightByDirection(SunLight.Base, normalize(SunLight.Direction), ShadowFactor);
}

void main()
{
	vec4 DirectionalLightColor = CalculateDirectionalLight();
	
	vec4 PointLightColor = CalculateAllPointLightsColor();
	vec4 SpotLightColor = CalculateAllSpotLightsColor();

	FragColor = texture(Texture, TexCoord) * (DirectionalLightColor + PointLightColor + SpotLightColor);
	//FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // normals debug line

	float gamma = 1.0;
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));

	/// Shadow map debug
	//vec3 ProjCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
	//ProjCoords = (ProjCoords * 0.5f) + 0.5f; // translate from (-1, 1) to (0, 1)
	//FragColor = vec4(texture(DirectionalShadowMap, ProjCoords.xy).rrr, 1.0);
}