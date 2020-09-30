#include "MeshModel.h"
#include "MeshReader.h"
#include "TextureUtils.h"

MeshModel::MeshModel()
{
	model = glm::mat4(1.0f);
}

void MeshModel::LoadFile(const std::string& modelFile,
	std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
	VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets)
{
	// Load in all our meshes
	std::vector<Mesh> meshList;
	MeshReader::loadFromBinary(modelFile, meshList,
		textureImages, textureImageMemory, textureImageViews, samplerDescriptorPool, samplerSetLayout, textureSampler, samplerDescriptorSets);

	this->meshList = meshList;
}

Mesh* MeshModel::getMesh(size_t index)
{
	if (index >= meshList.size())
	{
		throw std::runtime_error("Attempted to access invalid Mesh Index!");
	}

	return &meshList[index];
}

void MeshModel::setModel(glm::mat4 newModel)
{
	model = newModel;
}

void MeshModel::destroyMeshModel()
{
	for (auto& mesh : meshList)
	{
		mesh.destroyBuffers();
	}
}
