#pragma once

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 InitPosition,
			glm::vec3 InitWorldUp,
			GLfloat InitYaw,
			GLfloat InitPitch,
			GLfloat InitRoll,
			GLfloat InitMoveSpeed,
			GLfloat InitTurnSpeed);

	~Camera();

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();

	glm::mat4 GetViewMatrix();

	void KeyControl(bool* Keys, GLfloat DeltaTime);
	void MouseControl(GLfloat OffsetX, GLfloat OffsetY);

private:
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 UpVector;
	glm::vec3 RightVector;

	//TODO: move to world class ot utility class
	glm::vec3 WorldUpVector;

	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;

	// TODO: move to character class
	GLfloat MoveSpeed;
	GLfloat TurnSpeed;

	void Update();
};

