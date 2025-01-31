#include "Components/Camera.h"

#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 InitPosition,
				glm::vec3 InitWorldUp,
				GLfloat InitYaw,
				GLfloat InitPitch,
				GLfloat InitRoll)
{
	Position = InitPosition;
	WorldUpVector = InitWorldUp;

	RotationQuat = glm::quat(glm::vec3(glm::radians(InitPitch),
												glm::radians(InitYaw),
												glm::radians(InitRoll)));

	Direction = glm::normalize(RotationQuat * glm::vec3(0.0f, 0.0f, -1.0f));

	SetRotation(RotationQuat);
}

glm::vec3 Camera::GetPosition() const
{
	return Position;
}

glm::vec3 Camera::GetDirection() const
{
	return glm::normalize(Direction);
}

glm::quat Camera::GetRotation() const
{
	return RotationQuat;
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

void Camera::SetRotation(const glm::quat& Rotation)
{
	RotationQuat = Rotation;

	Direction = glm::normalize(RotationQuat * glm::vec3(0.0f, 0.0f, -1.0f));
	RightVector = glm::normalize(glm::cross(Direction, WorldUpVector));
	UpVector = glm::normalize(glm::cross(RightVector, Direction));
}

