#pragma once

#include <string>
#include <vector>

#include "DataStructures.h"

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	unsigned int materialIndex;
};

struct aiScene;
struct aiNode;
struct aiMesh;

class MeshCompiler
{
public:
	static void saveToBinary(const std::string& modelFile, const std::string& outputFile, std::vector<Mesh>& meshList);
private:
	static void LoadNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshList);
	static Mesh LoadMesh(const aiMesh* mesh, const aiScene* scene);
	static std::vector<std::string> LoadMaterials(const aiScene* scene);
};