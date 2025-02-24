#include "Core/SceneComponent.h"

SceneComponent::SceneComponent(std::string&& Name, float X, float Y, float Z)
		: Component(std::move(Name))
{
	SceneTransform.Location.x = X;
	SceneTransform.Location.y = Y;
	SceneTransform.Location.z = Z;

	SceneTransform.Rotation = glm::quat();
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

void SceneComponent::SetLocation(glm::vec3&& Location)
{
	SceneTransform.Location = Location;
}

void SceneComponent::SetRotation(float Pitch, float Yaw, float Roll)
{
	SceneTransform.Rotation = glm::quat(glm::radians(glm::vec3(Pitch, Yaw, Roll)));
}

void SceneComponent::SetRotation(glm::quat&& Rotation)
{
	SceneTransform.Rotation = Rotation;
}

glm::quat SceneComponent::GetRotation()
{
	return SceneTransform.Rotation;
}
