#pragma once
#include <iostream>
#include <vector>

#include "Mesh.h"

class MeshReader
{
public:
	static void loadFromBinary(const std::string& inputFile, std::vector<Mesh>& meshList,
		VkQueue& graphicsQueue, VkCommandPool& graphicsCommandPool,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
};

