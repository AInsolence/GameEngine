#include "Components/Transform.h"

Transform::Transform() : Rotation(glm::quat()), Location(glm::vec3(0.0f)), Scale(glm::vec3(1.0f))
{
}

Transform::Transform(const glm::vec3& Position, const glm::quat& Rotation, const glm::vec3& Scale)
	: Rotation(Rotation), Location(Position), Scale(Scale)
{
}
