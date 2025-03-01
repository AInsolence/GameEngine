#pragma once
#include <string>

// Base class for components (non-renderable)
class Component
{
public:
	Component() = default;
	Component(std::string&& Name) : Name(std::move(Name)) {}

	virtual ~Component() = default;

	virtual void Initialize(){}
	virtual void Update(float DeltaTime) {}

	std::string GetName() const;

	void SetName(std::string&& InName);
	void SetName(const std::string& InName);

private:
	std::string Name = "NONE";
};

