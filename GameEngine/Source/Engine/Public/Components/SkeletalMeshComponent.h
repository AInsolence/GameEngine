#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SceneComponent.h"
#include "Components/RenderableComponent.h"

class Mesh;
class Texture;

struct aiMesh;
struct aiScene;
struct aiNode;


class SkeletalMeshComponent : public SceneComponent, public RenderableComponent
{
public:
	SkeletalMeshComponent(std::string&& Name, std::string&& MeshName);

	void Render() const override;
	void RenderWithTexture(std::shared_ptr<Texture> CustomTexture) const;

	void Clear();

	bool LoadModel(const std::string& FilePath);

private:
	std::string MeshName;

	void LoadNode(const aiNode* Node, const aiScene* Scene);
	void LoadMesh(const aiMesh* InitMesh, const aiScene* Scene);
	void LoadMaterials(const aiScene* Scene);

	std::vector<std::shared_ptr<Mesh>> MeshUnits;
	std::vector<std::shared_ptr<Texture>> TextureUnits;
	std::vector<unsigned int> MaterialIndices;
};
