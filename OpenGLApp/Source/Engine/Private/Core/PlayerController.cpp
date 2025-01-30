#include "Core/PlayerController.h"

#include <common.hpp>
#include <GLFW/glfw3.h>

#include "Components/Camera.h"
#include "Core/MainWindow.h"
#include "Render/SpotLight.h"

PlayerController::PlayerController(std::shared_ptr<MainWindow> InitGameWindow,
									std::shared_ptr<Camera> InitPlayerCamera,
									GLfloat InitMoveSpeed,
									GLfloat InitTurnSpeed,
									GLfloat InitMouseSpeed)
{
	GameWindow = InitGameWindow;
	PlayerCamera = InitPlayerCamera;

	MoveSpeed = InitMoveSpeed;
	TurnSpeed = InitTurnSpeed;
	MouseSpeed = InitMouseSpeed;

	Yaw = InitPlayerCamera->GetRotation().x;
	Pitch = InitPlayerCamera->GetRotation().y;
	Roll = InitPlayerCamera->GetRotation().z;
}

void PlayerController::HandleInput(GLfloat DeltaTime)
{
	// Get and handle user input events
	glfwPollEvents();

	// Transfer inputs from main window to camera
	KeyControl(GameWindow->GetKeys(), DeltaTime);
	MouseControl(GameWindow->GetOffsetX(), GameWindow->GetOffsetY());

	if (GameWindow->GetKeys()[GLFW_KEY_L])
	{
		if (Flashlight)
		{
			Flashlight->Toggle();
			GameWindow->GetKeys()[GLFW_KEY_L] = false;
		}
	}
}

void PlayerController::SetFlashLight(std::shared_ptr<::SpotLight>& InitSpotLight)
{
	//TODO: temp solution for debug
	Flashlight = InitSpotLight;
}

void PlayerController::KeyControl(bool* Keys, GLfloat DeltaTime)
{
	const auto MoveVelocity = MoveSpeed * DeltaTime;
	const auto TurnVelocity = TurnSpeed * DeltaTime;

	if (Keys[GLFW_KEY_W])
	{
		PlayerCamera->MoveForward(MoveVelocity);
	}

	if (Keys[GLFW_KEY_S])
	{
		PlayerCamera->MoveForward(-MoveVelocity);
	}

	if (Keys[GLFW_KEY_D])
	{
		PlayerCamera->TurnRight(TurnVelocity);
	}

	if (Keys[GLFW_KEY_A])
	{
		PlayerCamera->TurnRight(-TurnVelocity);
	}
}

void PlayerController::MouseControl(GLfloat OffsetX, GLfloat OffsetY)
{
	OffsetX *= MouseSpeed;
	OffsetY *= MouseSpeed;

	Yaw += OffsetX;
	Pitch = glm::clamp(Pitch + OffsetY, -89.0f, 89.0f);

	PlayerCamera->Update(glm::vec3{Yaw, Pitch, Roll});
}
