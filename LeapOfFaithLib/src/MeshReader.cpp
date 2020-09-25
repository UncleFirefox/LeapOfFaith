#include "MeshReader.h"
#include "TextureUtils.h"

#include <fstream>
#include <string>

void MeshReader::loadFromBinary(const std::string& inputFile, std::vector<Mesh>& meshList,
	VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& graphicsCommandPool,
	std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
	VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets)
{
	std::ifstream file(inputFile, std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Could not open file " + inputFile + " for reading!");

	std::vector<std::string> textureNames;
	size_t texturesSize;
	file.read((char*)&texturesSize, sizeof(size_t));
	std::string texture;
	for (size_t i = 0; i < texturesSize; i++)
	{
		std::getline(file, texture, '\0');
		textureNames.push_back(texture);
	}

	// Conversion from the materials list IDs to our Descriptor Array IDs
	std::vector<int> matToTex(textureNames.size());

	//Loop over textureNames and create textures for them
	for (size_t i = 0; i < textureNames.size(); i++)
	{
		// If material had no texture, set '0' to indicate no texture, texture 0 will be reserved for a default texture
		if (textureNames[i].empty())
		{
			matToTex[i] = 0;
		}
		else
		{
			// Otherwise, create texture and set value to index of new texture
			matToTex[i] = TextureUtils::createTexture(textureNames[i], physicalDevice, logicalDevice, graphicsQueue, graphicsCommandPool, 
				textureImages,textureImageMemory, textureImageViews, samplerDescriptorPool, samplerSetLayout, textureSampler, samplerDescriptorSets);
		}
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

		meshList.push_back(Mesh(physicalDevice, logicalDevice, graphicsQueue, graphicsCommandPool, &vertices, &indices, matToTex[materialIndex]));
	}

	file.close();
}
