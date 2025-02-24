#pragma once
#include <fwd.hpp>
#include <detail/type_quat.hpp>

struct Transform
{
public:
	glm::vec3 Location;
	glm::quat Rotation;

	Transform(const glm::vec3& Position = glm::vec3(0.0f, 0.0f, 0.0f),
				const glm::quat& Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
};
