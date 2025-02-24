#pragma once
#include <string>

// Base class for components (non-renderable)
class Component
{
public:
	Component(std::string&& Name) : Name(std::move(Name)) {}

	virtual ~Component() = default;

	virtual void Update() {}
	std::string GetName() const;

private:
	std::string Name;
};

