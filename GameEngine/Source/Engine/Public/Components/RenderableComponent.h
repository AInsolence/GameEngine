#pragma once
#include "Component.h"

class RenderableComponent : virtual public Component
{
public:
	RenderableComponent(std::string&& Name) : Component(std::move(Name)) {}
	virtual void Render() const = 0;
};
