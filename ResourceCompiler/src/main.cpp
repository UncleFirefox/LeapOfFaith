#include <iostream>

#include <fstream>
#include <vector>

#include "DataStructures.h"

#include "MeshCompiler.h"

void loadFromBinary(const std::string& inputFile, std::vector<Mesh>& meshList, std::vector<std::string>& materials)
{
	std::ifstream file(inputFile, std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not open file " + inputFile + " for reading!");
	
	size_t materialsSize;
	file.read((char*)&materialsSize, sizeof(size_t));
	std::string material;
	for (size_t i = 0; i < materialsSize; i++)
	{
		std::getline(file, material, '\0');
		materials.push_back(material);
	}

	// Read how many meshes we have
	size_t meshSize;
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

	file.close();
}

int main()
{
	const std::string modelFile = "models/uh60.obj";
	const std::string outputFile = "uh60.bin";

	// TODO: Load materials
	std::vector<Mesh> writeList;
	MeshCompiler::saveToBinary(modelFile, outputFile, writeList);

	// Test function to verify data is being read correctly without the need of vulkan classes
	std::vector<Mesh> readList;
	std::vector<std::string> materials;
	loadFromBinary(outputFile, readList, materials);

	std::cout << "Compiling Complete!" << std::endl;
}