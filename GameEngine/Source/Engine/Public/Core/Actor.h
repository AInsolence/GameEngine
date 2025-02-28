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
	Actor();

	void SetRootComponent(std::shared_ptr<SceneComponent> Root);

	std::shared_ptr<SceneComponent> GetRootComponent() const;

	void AddComponent(std::shared_ptr<Component> Component);

	std::shared_ptr<Component> GetComponent(const std::string& Name);

	void Update(float DeltaTime);
	void Render();

	std::string GetName();
	void SetName(std::string&& InName);
	void SetName(const std::string& InName);

	std::vector<std::shared_ptr<RenderableComponent>>& GetAllRenderableComponents();

private:
	std::string Name = "Actor";

	std::shared_ptr<SceneComponent> RootComponent;
	std::unordered_map<std::string, std::shared_ptr<Component>> Components;
	std::vector<std::shared_ptr<RenderableComponent>> RenderableComponents;
};
