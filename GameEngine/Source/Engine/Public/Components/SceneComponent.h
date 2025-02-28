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

	void SetScale(float X, float Y, float Z);
	void SetScale(glm::vec3&& Position);

	glm::vec3 GetScale() const;
	void GetScale(float& OutX, float& OutY, float& OutZ) const;

	glm::vec3 GetLocation() const;
	void GetLocation(float& OutX, float& OutY, float& OutZ) const;

	glm::quat GetRotation() const;
	void GetRotation(float& OutPitch, float& OutYaw, float& OutRoll) const; // Rotation getter (in degrees)

private:
	Transform SceneTransform;
};
