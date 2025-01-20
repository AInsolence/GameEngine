#include "SkeletalMesh.h"

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>

#include "Mesh.h"
#include "Texture.h"

SkeletalMesh::SkeletalMesh(const std::string& FilePath)
{
	LoadModel(FilePath);
}

bool SkeletalMesh::LoadModel(const std::string& FilePath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(FilePath, aiProcess_Triangulate |
																aiProcess_FlipUVs |
																aiProcess_GenSmoothNormals |
																aiProcess_JoinIdenticalVertices);

	if (!Scene)
	{
		printf("Model [%s] failed to load. Error: %s", FilePath.c_str(), Importer.GetErrorString());
		return false;
	}

	LoadNode(Scene->mRootNode, Scene);
	LoadMaterials(Scene);

	return true;
}

void SkeletalMesh::Render() const
{
	for (unsigned int Index = 0; Index < MeshUnits.size(); ++Index)
	{
		const auto MaterialIndex = MaterialIndices.at(Index);

		if (MaterialIndex < TextureUnits.size() && TextureUnits.at(MaterialIndex))
		{
			TextureUnits.at(MaterialIndex)->Apply();
		}

		MeshUnits.at(Index)->Render();
	}
}

void SkeletalMesh::Clear()
{
	MeshUnits.clear();
	TextureUnits.clear();
	MaterialIndices.clear();
}

void SkeletalMesh::LoadNode(const aiNode* Node, const aiScene* Scene)
{
	for (unsigned int NodeIndex = 0; NodeIndex < Node->mNumMeshes; ++NodeIndex)
	{
		LoadMesh(Scene->mMeshes[Node->mMeshes[NodeIndex]], Scene);
	}

	for (unsigned int ChildIndex = 0; ChildIndex < Node->mNumChildren; ++ChildIndex)
	{
		LoadNode(Node->mChildren[ChildIndex], Scene);
	}
}

void SkeletalMesh::LoadMesh(const aiMesh* InitMesh, const aiScene* Scene)
{
	std::vector<GLfloat> Vertices;
	std::vector<unsigned int> Indices;

	for (unsigned int Index = 0; Index < InitMesh->mNumVertices; ++Index)
	{
		Vertices.insert(Vertices.end(), {InitMesh->mVertices[Index].x,
														InitMesh->mVertices[Index].y,
														InitMesh->mVertices[Index].z});
		if (InitMesh->mTextureCoords[0])
		{
			Vertices.insert(Vertices.end(), {InitMesh->mTextureCoords[0][Index].x,
															InitMesh->mTextureCoords[0][Index].y});
		}
		else
		{
			Vertices.insert(Vertices.end(), {0.0f, 0.0f});
		}

		Vertices.insert(Vertices.end(), {InitMesh->mNormals[Index].x,
														InitMesh->mNormals[Index].y,
														InitMesh->mNormals[Index].z});
	}

	for (unsigned int FaceIndex = 0; FaceIndex < InitMesh->mNumFaces; ++FaceIndex)
	{
		const aiFace Face = InitMesh->mFaces[FaceIndex];

		for(unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
		{
			Indices.push_back(Face.mIndices[Index]);
		}
	}

	MeshUnits.push_back(std::make_unique<Mesh>(Vertices, Indices));
	MaterialIndices.push_back(InitMesh->mMaterialIndex);
}

void SkeletalMesh::LoadMaterials(const aiScene* Scene)
{
	TextureUnits.clear();

	for (unsigned int MaterialIndex = 0; MaterialIndex < Scene->mNumMaterials; ++MaterialIndex)
	{
		const aiMaterial* Material = Scene->mMaterials[MaterialIndex];

		if (Material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString Path;

			if (Material->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
			{
				printf("Original texture path: %s", Path.C_Str());

				const size_t Index = std::string(Path.C_Str()).rfind("\\");
				std::string FilePath = std::string(Path.C_Str()).substr(Index + 1);

				std::string TexturePath = "Content/Textures/" + FilePath;

				auto LoadedTexture = std::make_shared<Texture>(TexturePath);

				if (!LoadedTexture->LoadTexture_RGBA() && !LoadedTexture->LoadTexture_RGB())
				{
					TextureUnits.emplace_back(std::make_shared<Texture>("Content/Textures/plain.jpg"));
				}
				else
				{
					TextureUnits.emplace_back(std::move(LoadedTexture));
				}
			}
			else
			{// load placeholder
				TextureUnits.emplace_back(std::make_shared<Texture>("Content/Textures/plain.jpg"));
			}
		}
		else
		{// load placeholder
			TextureUnits.emplace_back(std::make_shared<Texture>("Content/Textures/plain.jpg"));
		}
	}
}
