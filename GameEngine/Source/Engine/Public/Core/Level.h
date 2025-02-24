#pragma once

#include <memory>

#include "Actor.h"


class Level
{
public:
	Level() = default;

	void AddActor(const std::shared_ptr<Actor>& InActor);

	std::shared_ptr<Actor> GetActor(const std::string& Name) const;

	const std::vector<std::shared_ptr<Actor>>& GetAllActors() const;

	void Update(float DeltaTime);

	void SaveScene(const std::string& FilePath);
	void LoadScene(const std::string& FilePath);

private:
	std::vector<std::shared_ptr<Actor>> Actors;
	std::unordered_map<std::string, std::shared_ptr<Actor>> ActorMap;
};

