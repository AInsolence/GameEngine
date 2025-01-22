#include "Camera.h"
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

	MoveSpeed = InitMoveSpeed;
	TurnSpeed = InitTurnSpeed;

	Update();
}

Camera::~Camera()
{

}

glm::vec3 Camera::GetPosition() const
{
	return Position;
}

glm::vec3 Camera::GetDirection() const
{
	return glm::normalize(Direction);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Direction, UpVector);
}

void Camera::KeyControl(bool* Keys, GLfloat DeltaTime)
{
	const GLfloat Velocity = MoveSpeed * DeltaTime;

	if (Keys[GLFW_KEY_W])
	{
		Position += Direction * Velocity;
	}

	if (Keys[GLFW_KEY_S])
	{
		Position -= Direction * Velocity;
	}

	if (Keys[GLFW_KEY_A])
	{
		Position -= RightVector * Velocity;
	}

	if (Keys[GLFW_KEY_D])
	{
		Position += RightVector * Velocity;
	}
}

void Camera::MouseControl(GLfloat OffsetX, GLfloat OffsetY)
{
	OffsetX *= TurnSpeed;
	OffsetY *= TurnSpeed;

	Yaw += OffsetX;
	Pitch = glm::clamp(Pitch + OffsetY, -89.0f, 89.0f);

	Update();
}

void Camera::Update()
{
	Direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Direction.y = sin(glm::radians(Pitch));
	Direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Direction = glm::normalize(Direction);

	RightVector = glm::normalize(glm::cross(Direction, WorldUpVector));
	UpVector = glm::normalize(glm::cross(RightVector, Direction));
}
