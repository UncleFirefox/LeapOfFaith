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

	extern VkQueue* graphicsQueue;
	extern VkCommandPool* graphicsCommandPool;

	extern std::vector<VkImage>* textureImages;
	extern std::vector<VkImageView>*textureImageViews;
	extern std::vector<VkDeviceMemory>* textureImageMemory;

	extern VkDescriptorPool* samplerDescriptorPool;
	extern VkDescriptorSetLayout* samplerSetLayout;
	extern VkSampler* textureSampler;
	extern std::vector<VkDescriptorSet>* samplerDescriptorSets;
}

