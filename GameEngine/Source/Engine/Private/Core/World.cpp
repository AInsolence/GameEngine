#include "Core/World.h"

#include <gtc/type_ptr.hpp>

#include "Helper.h"
#include "Core/MainWindow.h"
#include "Components/Camera.h"
#include "Render/SceneRenderer.h"

World::World()
{
	Initialize();
}

void World::ProcessInput(GLfloat DeltaTime) const
{
	for (const auto& Controller : Controllers)
	{
		Controller->HandleInput(DeltaTime);
	}
}

void World::Initialize()
{
	GameWindow = std::make_shared<MainWindow>(2400, 1350);

	PlayerCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), 
										glm::vec3(0.0f, 1.0f, 0.0f),
										0.0f,
										0.0f,
										0.0f);

	FirstPlayerController = std::make_shared<PlayerController>(GameWindow, PlayerCamera);

	// TODO: temp for debug purposes
	/*if (!SpotLights.empty())
	{
		FirstPlayerController->SetFlashLight(SpotLights.at(0));
	}*/

	RegisterController(FirstPlayerController);

	//TODO substitute to real Level
	std::shared_ptr<Level> TestLevel = std::make_shared<Level>();

	WorldSceneRenderer = std::make_shared<SceneRenderer>(TestLevel);
	WorldSceneRenderer->Initialize(GameWindow);
}

bool World::IsGameOver()
{
	return GameWindow->GetShouldClose();
}

void World::RenderScene()
{
	WorldSceneRenderer->RenderScene(PlayerCamera);
}

void World::RegisterController(const std::shared_ptr<IInputController>& Controller)
{
	Controllers.push_back(Controller);
}
