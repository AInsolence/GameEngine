#pragma once

#include <GL/glew.h>

#include <glm.hpp>
#include <detail/type_quat.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 InitPosition,
			glm::vec3 InitWorldUp,
			GLfloat InitYaw,
			GLfloat InitPitch,
			GLfloat InitRoll);

	glm::vec3 GetPosition() const;
	glm::vec3 GetDirection() const;
	glm::quat GetRotation() const;

	void MoveForward(GLfloat Velocity);
	void TurnRight(GLfloat Velocity);
	void SetRotation(const glm::quat& Rotation);

	glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 Position;
	glm::quat RotationQuat;
	glm::vec3 UpVector;
	glm::vec3 Direction;
	glm::vec3 RightVector;
	glm::vec3 WorldUpVector;
};

