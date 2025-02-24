#include "Core/Actor.h"

#include "Core/SceneComponent.h"
#include "Core/RenderableComponent.h"


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

void Actor::Update()
{
	for (auto& Pair : Components)
	{
		Pair.second->Update();
	}
}

void Actor::Render()
{
	for (const auto& RenderComp : RenderableComponents)
	{
		RenderComp->Render();
	}
}
