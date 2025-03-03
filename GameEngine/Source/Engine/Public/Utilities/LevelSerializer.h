#pragma once

#include <memory>
#include <string>

class Level;
class Actor;
class PointLight;
class SpotLight;
class DirectionalLight;
class Grid;
class Skybox;

class LevelSerializer
{
public:
	// Static methods for saving and loading levels
	static void SaveLevel(const std::string& FilePath, const Level& CurrentLevel);
	static std::shared_ptr<Level> LoadLevel(const std::string& FilePath);

private:
	// Helper functions to save/load actors and lights
	static void SaveActor(std::ofstream& OutStream, const std::shared_ptr<Actor>& InActor);
	static void LoadActor(std::ifstream& inStream, Level& CurrentLevel);

	static void SavePointLight(std::ofstream& OutStream, const std::shared_ptr<PointLight>& InPointLight);
	static void LoadPointLight(std::ifstream& inStream, Level& level);

	static void SaveSpotLight(std::ofstream& OutStream, const std::shared_ptr<SpotLight>& InSpotLight);
	static void LoadSpotLight(std::ifstream& InStream, Level& CurrentLevel);

	static void SaveDirectionalLight(std::ofstream& OutStream, const std::shared_ptr<DirectionalLight>& InDirectionalLight);
	static void LoadDirectionalLight(std::ifstream& InStream, std::shared_ptr<Level>& CurrentLevel);

	static void SaveGrid(std::ofstream& OutStream, const std::unique_ptr<Grid>& InGrid);
	static void LoadGrid(std::ifstream& InStream, Level& CurrentLevel);

	static void SaveSkybox(std::ofstream& OutStream, const Level& CurrentLevel);
	static void LoadSkybox(std::ifstream& InStream, Level& CurrentLevel);

	// Helper functions for handling endianess
	static uint32_t ToLittleEndian(uint32_t value);
	static uint32_t FromLittleEndian(uint32_t value);
};