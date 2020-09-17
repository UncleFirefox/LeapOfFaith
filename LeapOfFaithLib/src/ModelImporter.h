#pragma once

#include <iostream>
#include <vector>

// Forward declaration
struct aiScene;

struct ImportResult
{
	std::vector<std::string> textureNames;
	const aiScene* scene;
};

class ModelImporter
{
public:
	static ImportResult createMeshModel(std::string modelFile);
	static std::vector<std::string> LoadMaterials(const aiScene* scene);
};

