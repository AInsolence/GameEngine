#include "Components/SceneComponent.h"

SceneComponent::SceneComponent() : Component("NONE")
{
}

SceneComponent::SceneComponent(std::string&& Name) : Component(std::move(Name))
{
}

void SceneComponent::SetTransform(const Transform& InitTransform)
{
	SceneTransform = InitTransform;
}

void SceneComponent::SetLocation(float X, float Y, float Z)
{
	SceneTransform.Location.x = X;
	SceneTransform.Location.y = Y;
	SceneTransform.Location.z = Z;
}

void SceneComponent::SetLocation(glm::vec3&& Position)
{
	SceneTransform.Location = Position;
}

glm::vec3 SceneComponent::GetScale() const
{
	return SceneTransform.Scale;
}

void SceneComponent::GetScale(float& OutX, float& OutY, float& OutZ) const
{
	OutX = SceneTransform.Scale.x;
	OutY = SceneTransform.Scale.y;
	OutZ = SceneTransform.Scale.z;
}

glm::vec3 SceneComponent::GetLocation() const
{
	return SceneTransform.Location;
}

void SceneComponent::GetLocation(float& OutX, float& OutY, float& OutZ) const
{
	OutX = SceneTransform.Location.x;
	OutY = SceneTransform.Location.y;
	OutZ = SceneTransform.Location.z;
}

void SceneComponent::SetRotation(float Pitch, float Yaw, float Roll)
{
	SceneTransform.Rotation = glm::quat(glm::radians(glm::vec3(Pitch, Yaw, Roll)));
}

void SceneComponent::SetRotation(glm::quat&& Rotation)
{
	SceneTransform.Rotation = Rotation;
}

void SceneComponent::SetScale(float X, float Y, float Z)
{
	SceneTransform.Scale.x = X;
	SceneTransform.Scale.y = Y;
	SceneTransform.Scale.z = Z;
}

void SceneComponent::SetScale(glm::vec3&& Position)
{
	SceneTransform.Scale = Position;
}

glm::quat SceneComponent::GetRotation() const
{
	return SceneTransform.Rotation;
}

void SceneComponent::GetRotation(float& OutPitch, float& OutYaw, float& OutRoll) const
{
	const glm::vec3 euler = glm::degrees(glm::eulerAngles(SceneTransform.Rotation));

	OutPitch = euler.x;
	OutYaw = euler.y;
	OutRoll = euler.z;
}
