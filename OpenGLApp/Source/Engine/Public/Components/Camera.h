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

	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;
	glm::vec3 GetRotation() const;

	void Update(glm::vec3 Rotation);
	void MoveForward(GLfloat Velocity);
	void TurnRight(GLfloat Velocity);

	glm::mat4 GetViewMatrix() const;

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
};

