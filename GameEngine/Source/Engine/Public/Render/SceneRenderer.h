#pragma once

#include <fwd.hpp>
#include <memory>

#include "Core/Level.h"

class Mesh;
class Camera;

class SceneRenderer
{
public:
	SceneRenderer(std::shared_ptr<Level> LevelIns);

	void Render(const std::shared_ptr<Camera>& MainCamera, const glm::mat4& ProjectionMatrix)
	{
		for (const auto& Actor : LevelInstance->GetAllActors())
		{
			Actor->Render();
		}
	}

private:
	std::shared_ptr<Level> LevelInstance;
};

