#include <iostream>

#include <fstream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "DataStructures.h"

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

Mesh LoadMesh(const aiMesh* mesh, const aiScene* scene)//, std::vector<int> matToTex)
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
	Mesh newMesh = { vertices, indices }; //, matToTex[mesh->mMaterialIndex]);

	return newMesh;
}

std::vector<Mesh> LoadNode(aiNode* node, const aiScene* scene)
{
	std::vector<Mesh> meshList;

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
		std::vector<Mesh> newList = LoadNode(node->mChildren[i], scene); //, matToTex);
		meshList.insert(meshList.end(), newList.begin(), newList.end());
	}

	return meshList;
}

void saveToBinary(const std::string& outputFile, const std::vector<Mesh>& meshList)
{
	std::ofstream file(outputFile, std::ios::out | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not open file " + outputFile + " for writing!");

	for (auto& mesh : meshList)
	{
		file.write((char*)&mesh.vertices, mesh.vertices.size() * sizeof(Vertex));
		file.write((char*)&mesh.indices, mesh.indices.size() * sizeof(uint32_t));
	}

	file.close();
}

int main()
{
	const std::string modelFile = "models/uh60.obj";
	const std::string outputFile = "uh60.bin";

	// Import model "scene"
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	// TODO: Load materials
	std::vector<Mesh> meshList = LoadNode(scene->mRootNode, scene);
	saveToBinary(outputFile, meshList);

	std::cout << "Compiling Complete!" << std::endl;
}