#include "Components/Component.h"

std::string Component::GetName() const
{
	return Name;
}

void Component::SetName(std::string&& InName)
{
	Name = std::move(InName);
}

void Component::SetName(const std::string& InName)
{
	Name = InName;
}
