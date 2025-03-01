#include "Components/SkeletalMeshComponent.h"

#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>

#include "Components/Mesh.h"
#include "Components/Texture.h"

#include "Helper.h"

SkeletalMeshComponent::SkeletalMeshComponent(std::string&& FilePath)
							: RenderableComponent("NONE")
{
	// TODO split download logic
	LoadModel(FilePath);
}

SkeletalMeshComponent::SkeletalMeshComponent(std::string&& Name, std::string&& FilePath)
							: RenderableComponent(std::move(Name))
{
	// TODO split download logic
	LoadModel(FilePath);
}

bool SkeletalMeshComponent::LoadModel(const std::string& FilePath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(FilePath, aiProcess_Triangulate |
																aiProcess_FlipUVs |
																aiProcess_GenSmoothNormals |
																aiProcess_JoinIdenticalVertices);

	if (!Scene)
	{
		printf("Model [%s] failed to load. Error: %s\n", FilePath.c_str(), Importer.GetErrorString());
		return false;
	}

	LoadNode(Scene->mRootNode, Scene);
	LoadMaterials(Scene);

	return true;
}

void SkeletalMeshComponent::SetCustomTexture(const std::shared_ptr<Texture>& Texture)
{
	CustomTexture = Texture;
}

void SkeletalMeshComponent::ClearCustomTexture()
{
	CustomTexture.reset();
}

void SkeletalMeshComponent::Render() const
{
	for (unsigned int Index = 0; Index < MeshUnits.size(); ++Index)
	{
		const auto MaterialIndex = MaterialIndices.at(Index);

		if (MaterialIndex < TextureUnits.size() && TextureUnits.at(MaterialIndex))
		{
			// TODO for debug, delete custom render
			if (CustomTexture)
			{
				CustomTexture->Apply();
			}
			else
			{
				TextureUnits.at(MaterialIndex)->Apply();
			}
		}

		MeshUnits.at(Index)->Render();
	}
}

void SkeletalMeshComponent::Clear()
{
	MeshUnits.clear();
	TextureUnits.clear();
	MaterialIndices.clear();
}

void SkeletalMeshComponent::LoadNode(const aiNode* Node, const aiScene* Scene)
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

void SkeletalMeshComponent::LoadMesh(const aiMesh* InitMesh, const aiScene* Scene)
{
	std::vector<GLfloat> Vertices;
	std::vector<unsigned int> Indices;

	Vertices.reserve(InitMesh->mNumVertices * 8);
	Indices.reserve(InitMesh->mNumFaces * 3);

	for (unsigned int Index = 0; Index < InitMesh->mNumVertices; ++Index)
	{
		Vertices.push_back(InitMesh->mVertices[Index].x);
		Vertices.push_back(InitMesh->mVertices[Index].y);
		Vertices.push_back(InitMesh->mVertices[Index].z);

		if (InitMesh->mTextureCoords[0])
		{
			Vertices.push_back(InitMesh->mTextureCoords[0][Index].x);
			Vertices.push_back(InitMesh->mTextureCoords[0][Index].y);
		}
		else
		{
			Vertices.push_back(0.0f);
			Vertices.push_back(0.0f);
		}

		Vertices.push_back(InitMesh->mNormals[Index].x);
		Vertices.push_back(InitMesh->mNormals[Index].y);
		Vertices.push_back(InitMesh->mNormals[Index].z);
	}

	for (unsigned int FaceIndex = 0; FaceIndex < InitMesh->mNumFaces; ++FaceIndex)
	{
		const aiFace Face = InitMesh->mFaces[FaceIndex];

		for(unsigned int Index = 0; Index < Face.mNumIndices; ++Index)
		{
			Indices.push_back(Face.mIndices[Index]);
		}
	}

	MeshUnits.push_back(std::make_shared<Mesh>(Vertices, Indices));
	MaterialIndices.push_back(InitMesh->mMaterialIndex);
}

void SkeletalMeshComponent::LoadMaterials(const aiScene* Scene)
{
	TextureUnits.clear();
	TextureUnits.reserve(Scene->mNumMaterials);

	for (unsigned int MaterialIndex = 0; MaterialIndex < Scene->mNumMaterials; ++MaterialIndex)
	{
		const aiMaterial* Material = Scene->mMaterials[MaterialIndex];
		bool TextureLoaded = false;

		for (int textureType = aiTextureType_NONE; textureType <= aiTextureType_UNKNOWN; ++textureType)
		{
			for (unsigned int Index = 0; Index < Material->GetTextureCount(static_cast<aiTextureType>(textureType)); ++Index)
			{
				aiString Path;

				if (Material->GetTexture(static_cast<aiTextureType>(textureType), Index, &Path) == AI_SUCCESS)
				{
					// printf("Original texture path (type %d): %s\n", textureType, Path.C_Str());
					if (Path.C_Str()[0] == '*') // Embedded texture check
					{
						const unsigned int TextureIndex = atoi(Path.C_Str() + 1);
						const aiTexture* EmbeddedTexture = Scene->mTextures[TextureIndex];

						if (EmbeddedTexture)
						{
							//std::string EmbeddedTextureName = "EmbeddedTexture_" + std::to_string(TextureIndex);
							//printf("Embedded texture found: %s\n", EmbeddedTextureName.c_str());
							auto LoadedTexture = std::make_shared<Texture>();

							if (EmbeddedTexture->pcData != nullptr && EmbeddedTexture->mWidth > 0 && EmbeddedTexture->mHeight > 0)
							{
								if (!LoadedTexture->LoadTextureFromMemory(reinterpret_cast<unsigned char*>(EmbeddedTexture->pcData),
																		EmbeddedTexture->mWidth * EmbeddedTexture->mHeight * 4))
								{
									printf("Failed to load embedded texture.\n");
									PlaceholderTexture->LoadTexture_RGBA();
									TextureUnits.emplace_back(PlaceholderTexture);
									continue;
								}
							}
							else
							{
								PlaceholderTexture->LoadTexture_RGBA();
								TextureUnits.emplace_back(PlaceholderTexture);
								continue;
							}

							TextureUnits.emplace_back(std::move(LoadedTexture));
							TextureLoaded = true;

							continue;
						}
					}
					else
					{
						const size_t Index = std::string(Path.C_Str()).find_last_of("/\\");
						std::string FilePath = std::string(Path.C_Str()).substr(Index + 1);

						std::string TexturePath = "Content/Textures/" + FilePath;
						//printf("Resolved texture path: %s\n", TexturePath.c_str());

						auto LoadedTexture = std::make_shared<Texture>(TexturePath);

						if (!LoadedTexture->LoadTexture_RGBA() && !LoadedTexture->LoadTexture_RGB())
						{
							printf("Failed to load texture: %s\n", TexturePath.c_str());
							PlaceholderTexture->LoadTexture_RGBA();
							TextureUnits.emplace_back(PlaceholderTexture);
						}
						else
						{
							TextureUnits.emplace_back(std::move(LoadedTexture));
						}

						TextureLoaded = true;
						continue;
					}
				}
			}
		}

		if (!TextureLoaded)
		{
			PlaceholderTexture->LoadTexture_RGBA();
			TextureUnits.emplace_back(PlaceholderTexture);
		}
	}
}
