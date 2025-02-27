#include "Components/Transform.h"

Transform::Transform(const glm::vec3& Position, const glm::quat& Rotation, const glm::vec3& Scale)
	: Rotation(Rotation), Location(Position), Scale(Scale)
{

}
