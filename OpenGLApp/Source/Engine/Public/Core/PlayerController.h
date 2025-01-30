#pragma once
#include <memory>
#include <GL/glew.h>

#include "IInputController.h"

class SpotLight;
class MainWindow;
class Camera;

class PlayerController : public IInputController
{
public:
	PlayerController(std::shared_ptr<MainWindow> InitGameWindow,
						std::shared_ptr<Camera> InitPlayerCamera,
						GLfloat InitMoveSpeed = 5.0f,
						GLfloat InitTurnSpeed = 5.0f,
						GLfloat InitMouseSpeed = 0.2f);

	void HandleInput(GLfloat DeltaTime) override;

	// TODO: temp for checking flashlight functionality
	void SetFlashLight(std::shared_ptr<::SpotLight>& InitSpotLight);
	
private:
	std::shared_ptr<SpotLight> Flashlight;
	std::shared_ptr<Camera> PlayerCamera;
	std::shared_ptr<MainWindow> GameWindow;

	double lastX = 400, lastY = 300;  // Центр экрана

	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll = 0.0f;

	// TODO: move to character class
	GLfloat MoveSpeed;
	GLfloat TurnSpeed;
	GLfloat MouseSpeed;

	void KeyControl(bool* Keys, GLfloat DeltaTime);
	void MouseControl(GLfloat OffsetX, GLfloat OffsetY);
};

