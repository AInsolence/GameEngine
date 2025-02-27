#pragma once
#include <fwd.hpp>
#include <detail/type_quat.hpp>

struct Transform
{
public:
	glm::quat Rotation;
	glm::vec3 Location;
	glm::vec3 Scale;

	Transform(const glm::vec3& Position = glm::vec3(0.0f, 0.0f, 0.0f),
				const glm::quat& Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
				const glm::vec3& Scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
