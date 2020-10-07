#pragma once
#include <vector>

#include <vulkan/vulkan_core.h>

typedef unsigned char stbi_uc;

namespace Utilities::Texture
{
	int createTexture(const char* fileName,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	VkImage createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propFlags,
		VkDeviceMemory* imageMemory);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	int createTextureImage(const char* fileName, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory);

	stbi_uc* loadTextureFile(const char* fileName, int* width, int* height, VkDeviceSize* imageSize);

	int createTextureDescriptor(VkImageView textureImage, VkDescriptorPool& samplerDescriptorPool, VkDevice& logicalDevice,
		VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
};