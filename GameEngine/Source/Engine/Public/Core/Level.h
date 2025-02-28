#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vec3.hpp>
#include <vec4.hpp>
#include <vector>
#include <GL/glew.h>

#include "Render/Shader.h"
#include "Components/Grid.h"
#include "Components/Skybox.h"

class Actor;
class SpotLight;
class PointLight;
class DirectionalLight;

class Level
{
public:
	Level();

	void Initialize();

	void AddActor(const std::shared_ptr<Actor>& InActor);
	std::shared_ptr<Actor> GetActor(const std::string& Name) const;
	const std::vector<std::shared_ptr<Actor>>& GetAllActors() const;

	void Update(float DeltaTime);

	void SaveScene(const std::string& FilePath);
	void LoadScene(const std::string& FilePath);

	void AddPointLight
	(
		glm::vec4 InitColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),  // Default color is white
		GLfloat InitAmbientIntensity = 0.1f,  // Default ambient intensity
		GLfloat InitDiffuseIntensity = 1.0f,  // Default diffuse intensity
		glm::vec3 InitPosition = glm::vec3(0.0f, 0.0f, 0.0f),  // Default position at the origin
		GLfloat InitExponent = 2.0f,  // Default exponent for light falloff
		GLfloat InitLinear = 0.09f,  // Default linear falloff coefficient
		GLfloat InitConstant = 1.0f,  // Default constant falloff coefficient
		GLfloat InitInnerRadius = 1.0f,  // Default inner radius for spotlight
		GLfloat InitOuterRadius = 10.0f,  // Default outer radius for spotlight
		GLfloat InitRadiusSharpness = 5.0f,  // Default sharpness of the radius for spotlight
		GLint InitShadowWidth = 1024,  // Default shadow map width
		GLint InitShadowHeight = 1024,  // Default shadow map height
		GLfloat InitNear = 0.01f,  // Default near plane for shadows
		GLfloat InitFar = 100.0f   // Default far plane for shadows
	);

	void AddSpotLight
	(
		glm::vec4 InitColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),  // Default color is white
		GLfloat InitAmbientIntensity = 0.1f,  // Default ambient intensity
		GLfloat InitDiffuseIntensity = 1.0f,  // Default diffuse intensity
		glm::vec3 InitPosition = glm::vec3(0.0f, 0.0f, 0.0f),  // Default position at the origin
		glm::vec3 InitDirection = glm::vec3(0.0f, -1.0f, 0.0f),  // Default direction pointing down
		GLfloat InitCutOffAngle = 12.5f,  // Default cutoff angle for the spotlight
		GLfloat InitExponent = 2.0f,  // Default exponent for light falloff
		GLfloat InitLinear = 0.09f,  // Default linear falloff coefficient
		GLfloat InitConstant = 1.0f,  // Default constant falloff coefficient
		GLfloat InitInnerRadius = 1.0f,  // Default inner radius for spotlight
		GLfloat InitOuterRadius = 10.0f,  // Default outer radius for spotlight
		GLfloat InitRadiusSharpness = 5.0f,  // Default sharpness of the radius for spotlight
		GLint InitShadowWidth = 1024,  // Default shadow map width
		GLint InitShadowHeight = 1024,  // Default shadow map height
		GLfloat InitNear = 0.0f,  // Default near plane for shadows
		GLfloat InitFar = 50.0f   // Default far plane for shadows
	);

	std::shared_ptr<DirectionalLight>& GetSunLight();
	std::vector<std::shared_ptr<PointLight>>& GetPointLights();
	std::vector<std::shared_ptr<SpotLight>>& GetSpotLights();
	
	std::unique_ptr<Skybox>& GetSkybox();
	std::unique_ptr<Grid>& GetEditorGrid();

private:
	void CreateSkybox();

	std::vector<std::shared_ptr<Actor>> Actors;
	std::unordered_map<std::string, std::shared_ptr<Actor>> ActorMap;

	std::shared_ptr<DirectionalLight> SunLight;
	std::vector<std::shared_ptr<PointLight>> PointLights;
	std::vector<std::shared_ptr<SpotLight>> SpotLights;

	std::unique_ptr<Skybox> Sky;
	std::unique_ptr<Grid> EditorGrid;
};

