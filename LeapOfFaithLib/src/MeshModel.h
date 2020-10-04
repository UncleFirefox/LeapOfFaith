#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

#include "Mesh.h"

class MeshModel
{
public:
	MeshModel();
	void LoadFile(const std::string& modelFile,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	inline size_t getMeshCount() const { return meshList.size(); }
	Mesh* getMesh(size_t index);

	inline glm::mat4 getModel() const { return model; }
	void setModel(glm::mat4 newModel);

	void destroyMeshModel();
private:
	std::vector<Mesh> meshList;
	glm::mat4 model;
};

