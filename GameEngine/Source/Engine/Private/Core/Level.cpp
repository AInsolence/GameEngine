#include "Core/Level.h"

void Level::AddActor(const std::shared_ptr<Actor>& InActor)
{
	std::string Name = InActor->GetName();

	if (ActorMap.contains(Name))
	{
		Name += "_" + std::to_string(ActorMap.size());
		InActor->SetName(Name);
	}

	Actors.push_back(InActor);
	ActorMap[Name] = InActor;
}

std::shared_ptr<Actor> Level::GetActor(const std::string& Name) const
{
	const auto ActorIt = ActorMap.find(Name);
	return (ActorIt != ActorMap.end()) ? ActorIt->second : nullptr;
}

const std::vector<std::shared_ptr<Actor>>& Level::GetAllActors() const
{
	return Actors;
}

void Level::Update(float DeltaTime)
{
	for (auto& Actor : Actors)
	{
		Actor->Update(DeltaTime);
	}
}
