#pragma once
#include <vulkan/vulkan.h>
#include <vector>

#include <iostream>

typedef unsigned char stbi_uc;

namespace TextureUtils
{
	int createTexture(const std::string& fileName,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	VkImage createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propFlags,
		VkDeviceMemory* imageMemory);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	int createTextureImage(const std::string& fileName, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory);

	stbi_uc* loadTextureFile(const std::string& fileName, int* width, int* height, VkDeviceSize* imageSize);

	int createTextureDescriptor(VkImageView textureImage, VkDescriptorPool& samplerDescriptorPool, VkDevice& logicalDevice,
		VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
};