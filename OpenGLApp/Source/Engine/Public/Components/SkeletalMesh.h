#pragma once

#include <memory>
#include <string>
#include <vector>

class Mesh;
class Texture;

struct aiMesh;
struct aiScene;
struct aiNode;


class SkeletalMesh
{
public:
	SkeletalMesh(const std::string& FilePath);

	bool LoadModel(const std::string& FilePath);
	void Render() const;
	void RenderWithTexture(std::shared_ptr<Texture> CustomTexture) const;
	void Clear();

private:
	void LoadNode(const aiNode* Node, const aiScene* Scene);
	void LoadMesh(const aiMesh* InitMesh, const aiScene* Scene);
	void LoadMaterials(const aiScene* Scene);

	std::vector<std::shared_ptr<Mesh>> MeshUnits;
	std::vector<std::shared_ptr<Texture>> TextureUnits;
	std::vector<unsigned int> MaterialIndices;
};

