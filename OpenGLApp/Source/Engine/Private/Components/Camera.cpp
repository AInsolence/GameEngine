#include "Components/Camera.h"

#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 InitPosition,
				glm::vec3 InitWorldUp,
				GLfloat InitYaw,
				GLfloat InitPitch,
				GLfloat InitRoll,
				GLfloat InitMoveSpeed,
				GLfloat InitTurnSpeed)
{
	Position = InitPosition;
	WorldUpVector = InitWorldUp;
	Yaw = InitYaw;
	Pitch = InitPitch;
	Roll = InitRoll;

	Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	Update(glm::vec3({Yaw, Pitch, Roll}));
}

glm::vec3 Camera::GetPosition() const
{
	return Position;
}

glm::vec3 Camera::GetDirection() const
{
	return glm::normalize(Direction);
}

glm::vec3 Camera::GetRotation() const
{
	return glm::vec3{Yaw, Pitch, Roll};
}


void Camera::MoveForward(GLfloat Velocity)
{
	Position += Direction * Velocity;
}

void Camera::TurnRight(GLfloat Velocity)
{
	Position += RightVector * Velocity;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Direction, UpVector);
}

void Camera::Update(glm::vec3 Rotation)
{
	Direction.x = cos(glm::radians(Rotation.x)) * cos(glm::radians(Pitch));
	Direction.y = sin(glm::radians(Rotation.y));
	Direction.z = sin(glm::radians(Rotation.x)) * cos(glm::radians(Pitch));

	Direction = glm::normalize(Direction);

	RightVector = glm::normalize(glm::cross(Direction, WorldUpVector));
	UpVector = glm::normalize(glm::cross(RightVector, Direction));
}

