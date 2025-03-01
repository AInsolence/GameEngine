#pragma once

#include <memory>
#include <vector>


#include "Core/PlayerController.h"


class SceneRenderer;

class World
{
public:
	World();
	void Initialize();

	bool IsGameOver();

	void RenderScene();
	void ProcessInput(GLfloat DeltaTime) const;

	void RegisterController(const std::shared_ptr<IInputController>& Controller);

private:
	std::shared_ptr<MainWindow> GameWindow;
	std::shared_ptr<SceneRenderer> WorldSceneRenderer;

	std::shared_ptr<Camera> PlayerCamera;
	std::shared_ptr<PlayerController> FirstPlayerController;
	
	std::vector<std::shared_ptr<IInputController>> Controllers;
};
