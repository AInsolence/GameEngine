#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Component;
class SceneComponent;
class RenderableComponent;

class Actor
{
public:
	Actor() : RootComponent(nullptr){}

	void SetRootComponent(std::shared_ptr<SceneComponent> Root);

	std::shared_ptr<SceneComponent> GetRootComponent() const;

	void AddComponent(std::shared_ptr<Component> Component);

	std::shared_ptr<Component> GetComponent(const std::string& Name);

	void Update();
	void Render();

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> Components;
	std::vector<std::shared_ptr<RenderableComponent>> RenderableComponents;
	std::shared_ptr<SceneComponent> RootComponent;
};
