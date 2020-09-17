#pragma once

#include <iostream>
#include <vector>

#include "TempStructures.h"

class ModelImporter
{
public:
	static ImportResult createMeshModel(std::string modelFile);
	static std::vector<std::string> LoadMaterials(const aiScene* scene);
};

