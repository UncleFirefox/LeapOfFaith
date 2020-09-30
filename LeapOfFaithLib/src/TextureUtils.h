#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include <iostream>

typedef unsigned char stbi_uc;

class TextureUtils
{
public:
	static int createTexture(const std::string& fileName,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	static VkImage createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propFlags,
		VkDeviceMemory* imageMemory);

	static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	static int createTextureImage(const std::string& fileName, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory);

	static stbi_uc* loadTextureFile(const std::string& fileName, int* width, int* height, VkDeviceSize* imageSize);

	static int createTextureDescriptor(VkImageView textureImage, VkDescriptorPool& samplerDescriptorPool, VkDevice& logicalDevice,
		VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
};

