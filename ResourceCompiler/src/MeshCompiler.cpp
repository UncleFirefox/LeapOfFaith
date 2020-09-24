#include "MeshCompiler.h"

#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void MeshCompiler::saveToBinary(const std::string& modelFile, const std::string& outputFile, std::vector<Mesh>& meshList)
{
	//Import model "scene"
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	LoadNode(scene->mRootNode, scene, meshList);

	std::ofstream file(outputFile, std::ios::out | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not open file " + outputFile + " for writing!");

	std::vector<std::string> materials = LoadMaterials(scene);
	size_t materialsSize = materials.size();
	file.write(reinterpret_cast<const char*>(&materialsSize), sizeof(size_t));
	for (auto& material : materials)
	{
		file.write(material.c_str(), material.size());
		file.write("\0", sizeof(char));
	}

	size_t meshSize = meshList.size();
	file.write(reinterpret_cast<const char*>(&meshSize), sizeof(size_t));

	size_t numVertices;
	size_t numIndices;
	for (auto& mesh : meshList)
	{
		numVertices = mesh.vertices.size();
		file.write(reinterpret_cast<const char*>(&numVertices), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&mesh.vertices[0]), numVertices * sizeof(Vertex));

		numIndices = mesh.indices.size();
		file.write(reinterpret_cast<const char*>(&numIndices), sizeof(size_t));
		file.write(reinterpret_cast<const char*>(&mesh.indices[0]), numIndices * sizeof(uint32_t));

		file.write(reinterpret_cast<const char*>(&mesh.materialIndex), sizeof(unsigned int));
	}

	file.close();
}

void MeshCompiler::LoadNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshList)
{
	// Go through each mesh at this node and create it, then add it to our meshList
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		meshList.push_back(
			LoadMesh(scene->mMeshes[node->mMeshes[i]], scene)// , matToTex)
		);
	}

	// Go through each node attached to this node and load it, then append their meshes to this node's mesh list
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, meshList); //, matToTex);
	}
}

Mesh MeshCompiler::LoadMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// Resize vertex list to hold all vertices for mesh
	vertices.resize(mesh->mNumVertices);

	// Go through each vertex and copy it across to our vertices
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// Set position
		vertices[i].pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

		// Set tex coords (if they exist)
		if (mesh->mTextureCoords[0])
		{
			vertices[i].tex = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}
		else
		{
			vertices[i].tex = { 0.0f, 0.0f };
		}

		// Set color (just use white for now)
		vertices[i].col = { 1.0f, 1.0f, 1.0f };
	}

	// Iterate over indices through faces and copy across
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		// Get a face
		aiFace face = mesh->mFaces[i];

		// Go through faces indices and add to list
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create new mesh with details and return it
	Mesh newMesh = { vertices, indices, mesh->mMaterialIndex }; //, matToTex[mesh->mMaterialIndex]);

	return newMesh;
}

std::vector<std::string> MeshCompiler::LoadMaterials(const aiScene* scene)
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
