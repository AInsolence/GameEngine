#include "Core/Actor.h"

#include "Core/SceneComponent.h"
#include "Core/RenderableComponent.h"


Actor::Actor() : RootComponent(nullptr)
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

void Actor::Update(float DeltaTime)
{
	for (auto& Pair : Components)
	{
		Pair.second->Update(DeltaTime);
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

void Actor::SetName(std::string&& InName)
{
	Name = std::move(InName);
}

std::string Actor::GetName()
{
	return Name;
}
