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
	unsigned int materialIndex;
};

std::vector<std::string> LoadMaterials(const aiScene* scene)
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
	Mesh newMesh = { vertices, indices, mesh->mMaterialIndex }; //, matToTex[mesh->mMaterialIndex]);

	return newMesh;
}

void LoadNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshList)
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

void saveToBinary(const std::string& modelFile, const std::string& outputFile, std::vector<Mesh>& meshList)
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

	std::vector<std::string> materials = LoadMaterials(scene);
	size_t materialsSize = materials.size();
	file.write(reinterpret_cast<const char*>(&materialsSize), sizeof(size_t));
	for (auto& material : materials)
	{
		file.write(material.c_str(), material.size());
		file.write("\0", sizeof(char));
	}

	file.close();
}

void loadFromBinary(const std::string& inputFile, std::vector<Mesh>& meshList, std::vector<std::string>& materials)
{
	std::ifstream file(inputFile, std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not open file " + inputFile + " for reading!");
	
	// Read how many meshes we have
	size_t meshSize;
	file.seekg(0, std::ios::beg);
	file.read((char*)&meshSize, sizeof(size_t));

	size_t vertexSize;
	size_t indexSize;
	Vertex v;
	uint32_t index;
	unsigned int materialIndex;

	for (size_t i = 0; i < meshSize; i++)
	{
		file.read((char*)&vertexSize, sizeof(size_t));
		std::vector<Vertex> vertices;
		vertices.reserve(vertexSize);

		for (size_t j = 0; j < vertexSize; j++)
		{
			file.read((char*)&v, sizeof(Vertex));
			vertices.push_back(v);
		}

		file.read((char*)&indexSize, sizeof(size_t));
		std::vector<uint32_t> indices;
		indices.reserve(indexSize);
		
		for (size_t j = 0; j < indexSize; j++)
		{
			file.read((char*)&index, sizeof(uint32_t));
			indices.push_back(index);
		}

		file.read((char*)&materialIndex, sizeof(unsigned int));

		meshList.push_back(Mesh{ vertices, indices, materialIndex });
	}

	size_t materialsSize;
	file.read((char*)&materialsSize, sizeof(size_t));
	std::string material;
	for (size_t i = 0; i < materialsSize; i++)
	{
		std::getline(file, material, '\0');
		materials.push_back(material);
	}

	file.close();
}

int main()
{
	const std::string modelFile = "models/uh60.obj";
	const std::string outputFile = "uh60.bin";

	// TODO: Load materials
	std::vector<Mesh> writeList;
	saveToBinary(modelFile, outputFile, writeList);

	std::vector<Mesh> readList;
	std::vector<std::string> materials;
	loadFromBinary(outputFile, readList, materials);

	std::cout << "Compiling Complete!" << std::endl;
}