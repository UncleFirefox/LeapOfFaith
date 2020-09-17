#include "ModelImporter.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ImportResult ModelImporter::createMeshModel(std::string modelFile)
{
	// Import model "scene"
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	// Get vector of all materials with 1:1 ID placement
	std::vector<std::string> textureNames = LoadMaterials(scene);

	return ImportResult{ textureNames, scene };
}

std::vector<std::string> ModelImporter::LoadMaterials(const aiScene* scene)
{
	// Create 1:1 sized list of textures
	std::vector<std::string> textureList(scene->mNumMaterials);

	// Go through each material and copy its texture file name (if it exists)
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		// Get the material
		aiMaterial* material = scene->mMaterials[i];

		// Initialize the texture to empty string (will be replaced if texture exists)
		textureList[i] = "";

		// Check for a Diffuse Texture (standard details texture)
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			// Get path of the texture file
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Cut off any directory information already present
				int idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);

				textureList[i] = fileName;
			}
		}
	}

	return textureList;
}
