#include "Core/Actor.h"

#include <ranges>

#include "Components/SceneComponent.h"
#include "Components/RenderableComponent.h"


Actor::Actor() : RootComponent(nullptr)
{
}

Actor::Actor(std::string&& Name) :  Name(std::move(Name)), RootComponent(nullptr)
{
}

void Actor::SetRootComponent(std::shared_ptr<SceneComponent> Root)
{
	RootComponent = Root;
	AddComponent(Root);
}

std::shared_ptr<SceneComponent> Actor::GetRootComponent() const
{
	return RootComponent;
}

void Actor::AddComponent(std::shared_ptr<Component> Component)
{
	std::string CompName = Component->GetName();

	if (Components.contains(CompName))
	{
		CompName += "_" + std::to_string(Components.size());
		Component->SetName(CompName);
	}

	Components[Component->GetName()] = Component;

	if (const auto& RenderComp = std::dynamic_pointer_cast<RenderableComponent>(Component))
	{
		RenderableComponents.push_back(RenderComp);
	}
}

std::shared_ptr<Component> Actor::GetComponent(const std::string& Name)
{
	const auto Comp = Components.find(Name);
	return (Comp != Components.end()) ? Comp->second : nullptr;
}

void Actor::Initialize()
{
	for (const auto& Comp : Components | std::views::values)
	{
		Comp->Initialize();
	}
}

void Actor::Update(float DeltaTime)
{
	for (const auto& Comp : Components | std::views::values)
	{
		Comp->Update(DeltaTime);
	}
}

void Actor::Render()
{
	for (const auto& RenderComp : RenderableComponents)
	{
		RenderComp->Render();
	}
}

void Actor::SetName(const std::string& InName)
{
	Name = InName;
}

std::vector<std::shared_ptr<RenderableComponent>>& Actor::GetAllRenderableComponents()
{
	return RenderableComponents;
}

void Actor::SetName(std::string&& InName)
{
	Name = std::move(InName);
}

std::string Actor::GetName()
{
	return Name;
}
