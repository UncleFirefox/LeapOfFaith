#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Globals {
	struct MainDevice {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	};

	extern MainDevice* mainDevice;

	VkQueue graphicsQueue;
	VkCommandPool graphicsCommandPool;

	std::vector<VkImage> textureImages;
	std::vector<VkImageView> textureImageViews;
	std::vector<VkDeviceMemory> textureImageMemory;

	VkDescriptorPool samplerDescriptorPool;
	VkDescriptorSetLayout samplerSetLayout;
	VkSampler textureSampler;
	std::vector<VkDescriptorSet> samplerDescriptorSets;
}

