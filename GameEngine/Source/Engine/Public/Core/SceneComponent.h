#pragma once

#include <glm.hpp>
#include <detail/type_quat.hpp>
#include <gtc/quaternion.hpp>

#include "Component.h"
#include "Transform.h"

class SceneComponent : public Component
{
public:
	SceneComponent(std::string&& Name, float X = 0.0f, float Y = 0.0f, float Z = 0.0f);

	void SetTransform(const Transform& InitTransform);

	void SetLocation(float X, float Y, float Z);
	void SetLocation(glm::vec3&& Position);

	void SetRotation(float Pitch, float Yaw, float Roll);
	void SetRotation(glm::quat&& Rotation);

	void GetLocation(float& OutX, float& OutY, float& OutZ) const
	{
		OutX = SceneTransform.Location.x;
		OutY = SceneTransform.Location.y;
		OutZ = SceneTransform.Location.z;
	}

	glm::quat GetRotation();

	// Rotation getter (in degrees)
	void GetRotation(float& OutPitch, float& OutYaw, float& OutRoll) const
	{
		const glm::vec3 euler = glm::degrees(glm::eulerAngles(SceneTransform.Rotation));
		OutPitch = euler.x;
		OutYaw = euler.y;
		OutRoll = euler.z;
	}

private:
	Transform SceneTransform;
};
