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
	std::shared_ptr<SceneRenderer> WorldSceneRenderer;
	std::shared_ptr<MainWindow> GameWindow;

	std::shared_ptr<Camera> PlayerCamera;
	std::shared_ptr<PlayerController> FirstPlayerController;
	
	std::vector<std::shared_ptr<IInputController>> Controllers;
};

//#pragma once
//#include <memory>
//#include "Scene.h"
//#include "SceneRenderer.h"
//#include "Core/PlayerController.h"
//
//class World {
//public:
//	World();
//	void Initialize();
//
//	void ProcessInput(GLfloat DeltaTime) const;
//	void Update(GLfloat DeltaTime);
//	void Render();
//
//	void RegisterController(const std::shared_ptr<IInputController>& Controller);
//
//private:
//	std::shared_ptr<Scene> GameScene;
//	std::shared_ptr<SceneRenderer> SceneRender;
//	std::shared_ptr<Camera> PlayerCamera;
//	std::shared_ptr<PlayerController> FirstPlayerController;
//};