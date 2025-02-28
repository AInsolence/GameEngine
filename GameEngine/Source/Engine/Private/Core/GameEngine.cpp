#include "Core/GameEngine.h"

#include <chrono>
#include <GLFW/glfw3.h>

void GameEngine::Run()
{
	WorldInstance = std::make_shared<World>();

	Start();
}

GameEngine::GameEngine()
{

}

void GameEngine::Start()
{
	while (!WorldInstance->IsGameOver())
	{
		const GLfloat CurrentTime = glfwGetTime(); // SDL_GetPerformanceCounter();
		DeltaTime = CurrentTime - LastTime; // (now - LastTime)*1000/SDL_GetPerformanceFrequency();
		LastTime = CurrentTime;

		WorldInstance->ProcessInput(DeltaTime);
		WorldInstance->RenderScene();
	}
}
