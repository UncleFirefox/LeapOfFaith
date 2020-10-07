#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Globals.h"
#include "Texture.h"

#include "Vulkan.h"

namespace Utilities::Texture
{
	int createTexture(const std::string& fileName, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets)
	{
		// Create Texture Image and get its location in array
		int textureImageLoc = createTextureImage(fileName, textureImages, textureImageMemory);

		// Create image view and add to list
		VkImageView imageView = createImageView(textureImages[textureImageLoc], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
		textureImageViews.push_back(imageView);

		// Create texture descriptor
		int descriptorLoc = createTextureDescriptor(imageView, samplerDescriptorPool, Globals::vkContext->logicalDevice, samplerSetLayout, textureSampler, samplerDescriptorSets);

		// Return location of set with texture
		return descriptorLoc;
	}

	VkImage createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propFlags,
		VkDeviceMemory* imageMemory)
	{
		// Create image
		// Image creation info
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D; // Type of image (1D, 2D, 3D)
		imageCreateInfo.extent.width = width; // w of image extent
		imageCreateInfo.extent.height = height; // h of image extent
		imageCreateInfo.extent.depth = 1; // Dep`th of image (just 1, no 3D aspect)
		imageCreateInfo.mipLevels = 1; // Number of mipmap levels
		imageCreateInfo.arrayLayers = 1; // Number of levels in image array
		imageCreateInfo.format = format; // Format type of image
		imageCreateInfo.tiling; // how image data should be tiled (arranged)
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout of image data on creation
		imageCreateInfo.usage = usageFlags; // Bit flags defining what image will be used for
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT; // Number of samples for multi-sampling
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // Whether image can be shared between queues

		VkImage image;
		VkResult result = vkCreateImage(Globals::vkContext->logicalDevice, &imageCreateInfo, nullptr, &image);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create an Image!");
		}

		// Create memory for image
		// Get memory requirements for type of image
		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(Globals::vkContext->logicalDevice, image, &memoryRequirements);

		// Allocate memory using image requirements and user defined properties
		VkMemoryAllocateInfo memoryAllocInfo = {};
		memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocInfo.allocationSize = memoryRequirements.size;
		memoryAllocInfo.memoryTypeIndex = Vulkan::findMemoryTypeIndex(Globals::vkContext->physicalDevice, memoryRequirements.memoryTypeBits, propFlags);

		result = vkAllocateMemory(Globals::vkContext->logicalDevice, &memoryAllocInfo, nullptr, imageMemory);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate memory for image!");
		}

		// Connect memory to image
		vkBindImageMemory(Globals::vkContext->logicalDevice, image, *imageMemory, 0);

		return image;
	}

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.image = image; // Image to create view for
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Type of image (1D, 2d, 3D, Cube, etc)
		viewCreateInfo.format = format; // Format of image data
		viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; // Allows remmapping of rgba components to other rgba values
		viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		// Subresources allow the view to view only a part of an image
		viewCreateInfo.subresourceRange.aspectMask = aspectFlags; // which aspect of the image to view, (COLOR_BIT etc)
		viewCreateInfo.subresourceRange.baseMipLevel = 0; // start mipmap level to view from
		viewCreateInfo.subresourceRange.levelCount = 1; // Number of mipmap levels to view
		viewCreateInfo.subresourceRange.baseArrayLayer = 0; // Start array level to view from
		viewCreateInfo.subresourceRange.layerCount = 1; // Number of array levels to view

		// Create image view and return it
		VkImageView imageView;
		VkResult result = vkCreateImageView(Globals::vkContext->logicalDevice, &viewCreateInfo, nullptr, &imageView);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create an Image View!");
		}

		return imageView;
	}

	int createTextureImage(const std::string& fileName, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory)
	{
		// Load image file
		int width, height;
		VkDeviceSize imageSize;
		stbi_uc* imageData = loadTextureFile(fileName, &width, &height, &imageSize);

		// Create staging buffer to hold loaded data, ready to copy to device
		VkBuffer imageStagingBuffer;
		VkDeviceMemory imageStagingBufferMemory;
		Vulkan::createBuffer(Globals::vkContext->physicalDevice, Globals::vkContext->logicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		                     &imageStagingBuffer, &imageStagingBufferMemory);

		// Copy image to staging buffer
		void* data;
		vkMapMemory(Globals::vkContext->logicalDevice, imageStagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, imageData, static_cast<size_t>(imageSize));
		vkUnmapMemory(Globals::vkContext->logicalDevice, imageStagingBufferMemory);

		// Free original image data
		stbi_image_free(imageData);

		// Create image to hold final texture
		VkImage texImage;
		VkDeviceMemory texImageMemory;
		texImage = createImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &texImageMemory);

		// Copy data to image

		// Transition image to be DST for copy operation
		Vulkan::transitionImageLayout(Globals::vkContext->logicalDevice, Globals::vkContext->graphicsQueue, Globals::vkContext->graphicsCommandPool,
		                              texImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		// Copy image data
		Vulkan::copyImageBuffer(Globals::vkContext->logicalDevice, Globals::vkContext->graphicsQueue, Globals::vkContext->graphicsCommandPool, imageStagingBuffer, texImage, width, height);

		// Transition image to be shader readable for shader usage
		Vulkan::transitionImageLayout(Globals::vkContext->logicalDevice, Globals::vkContext->graphicsQueue, Globals::vkContext->graphicsCommandPool,
		                              texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Add texture data to vector for reference
		textureImages.push_back(texImage);
		textureImageMemory.push_back(texImageMemory);

		// Destroy staging buffer
		vkDestroyBuffer(Globals::vkContext->logicalDevice, imageStagingBuffer, nullptr);
		vkFreeMemory(Globals::vkContext->logicalDevice, imageStagingBufferMemory, nullptr);

		// Return index to the new image
		return textureImages.size() - 1;
	}

	stbi_uc* loadTextureFile(const std::string& fileName, int* width, int* height, VkDeviceSize* imageSize)
	{
		// Number of channels image uses
		int channels;

		// Load pixel data for image
		std::string fileLoc = "textures/" + fileName;
		stbi_uc* image = stbi_load(fileLoc.c_str(), width, height, &channels, STBI_rgb_alpha);

		if (!image)
		{
			throw std::runtime_error("Failed to load a Texture file! (" + fileName + ")");
		}

		// Calculate image size using given and known data
		*imageSize = (*width) * (*height) * 4;

		return image;
	}

	int createTextureDescriptor(VkImageView textureImage, VkDescriptorPool& samplerDescriptorPool, VkDevice& logicalDevice,
		VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets)
	{
		VkDescriptorSet descriptorSet;

		// Descriptor set allocation info
		VkDescriptorSetAllocateInfo setAllocInfo = {};
		setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		setAllocInfo.descriptorPool = samplerDescriptorPool;
		setAllocInfo.descriptorSetCount = 1;
		setAllocInfo.pSetLayouts = &samplerSetLayout;

		// Allocate Descriptor Sets
		VkResult result = vkAllocateDescriptorSets(logicalDevice, &setAllocInfo, &descriptorSet);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate Texture Descriptor Sets!");
		}

		// Texture Image info
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //Image layout when in use
		imageInfo.imageView = textureImage; // Image to bind to set
		imageInfo.sampler = textureSampler; // Sampler to use for set

		// Descriptor write info
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		// Update new descriptor set
		vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);

		// Add descriptor set to list
		samplerDescriptorSets.push_back(descriptorSet);

		// Return descriptor set location
		return samplerDescriptorSets.size() - 1;
	}
}
