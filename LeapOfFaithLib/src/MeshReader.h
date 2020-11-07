#pragma once
#include <vector>

#include "Mesh.h"

namespace MeshReader
{
	void loadFromBinary(const char* inputFile, std::vector<Mesh>& meshList,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
};

