#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

#include "../DataStructures.h"

const int MAX_FRAME_DRAWS = 2;
const int MAX_OBJECTS = 20;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Indices (locations) of Queue Families (if they exist at all)
struct QueueFamilyIndices
{
	int graphicsFamily = -1; // Location of Graphics Queue family
	int presentationFamily = -1; // Location of presentation queue family

	// Check if queue families are valid
	bool isValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities; // Surface properties, e.g. image size/extent...
	std::vector<VkSurfaceFormatKHR> formats; // Surface image formats, e.g. RGBA and size of each colour
	std::vector<VkPresentModeKHR> presentationModes; // How images should be presented to screen
};

struct SwapchainImage {
	VkImage image;
	VkImageView imageView;
};

namespace Utilities::Vulkan
{
	static uint32_t findMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes, VkMemoryPropertyFlags properties)
	{
		// Get propeties of physical device memory
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (((allowedTypes) & (1 << i)) // Index of memory type must match corresponding bit in allowedTypes
				&& (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) // Desired property bit flags are part of memory types property flags
			{
				// This memory type is valid, so return its index
				return i;
			}
		}
	}

	static void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage,
		VkMemoryPropertyFlags bufferProperties, VkBuffer* buffer, VkDeviceMemory* bufferMemory)
	{
		// CREATE VERTEX BUFFER
	// Information to create a buffer (doesn't include assigning memory)
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(Vertex) * bufferSize;		// Size of buffer (size of 1 vertex * number of vertices)
		bufferInfo.usage = bufferUsage;		// Multiple types of buffer possible
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;			// Similar to Swap Chain images, can share vertex buffers

		VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, buffer);
		if (result == VK_SUCCESS && "Failed to create a Vertex Buffer!");

		// GET BUFFER MEMORY REQUIREMENTS
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);

		// ALLOCATE MEMORY TO BUFFER
		VkMemoryAllocateInfo memoryAllocInfo = {};
		memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocInfo.allocationSize = memRequirements.size;
		memoryAllocInfo.memoryTypeIndex = findMemoryTypeIndex(physicalDevice, memRequirements.memoryTypeBits,		// Index of memory type on Physical Device that has required bit flags
			bufferProperties);			// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT	: CPU can interact with memory
																									// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT	: Allows placement of data straight into buffer after mapping (otherwise would have to specify manually)
		// Allocate memory to VkDeviceMemory
		result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, bufferMemory);
		assert(result == VK_SUCCESS && "Failed to allocate Vertex Buffer Memory!");

		// Allocate memory to given vertex buffer
		vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
	}

	static VkCommandBuffer beginCommandBuffer(VkDevice device, VkCommandPool commandPool)
	{
		// Command buffer to hold transfer commands
		VkCommandBuffer commandBuffer;

		// Command Buffer details
		VkCommandBufferAllocateInfo allocInfo = { };
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		// Allocate command buffer from pool
		VkResult result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
		assert(result == VK_SUCCESS && "Failed to allocate Command Buffers!");

		// Information to begin the command buffer record
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // We're only using the command buffer once

		// Begin recording transfer commands
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	static void endAndSubmitCommandBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkCommandBuffer commandBuffer)
	{
		// End commands
		vkEndCommandBuffer(commandBuffer);

		// Queue submission information
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		// Submit transfer command to transfer queue and wait until it finishes
		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(queue);

		// Free temporary command buffer back to pool
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

	static void copyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
	{
		// Create the buffer
		VkCommandBuffer transferCommandBuffer = beginCommandBuffer(device, transferCommandPool);

		// Region of data to copy from and to
		VkBufferCopy bufferCopyRegion = {};
		bufferCopyRegion.srcOffset = 0;
		bufferCopyRegion.dstOffset = 0;
		bufferCopyRegion.size = bufferSize;

		// Command to copy src buffer to dst buffer
		vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion);

		endAndSubmitCommandBuffer(device, transferCommandPool, transferQueue, transferCommandBuffer);
	}

	static void copyImageBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
		VkBuffer srcBuffer, VkImage image, uint32_t width, uint32_t height)
	{
		// Create the buffer
		VkCommandBuffer transferCommandBuffer = beginCommandBuffer(device, transferCommandPool);

		VkBufferImageCopy imageRegion = {};
		imageRegion.bufferOffset = 0; // Offset into data
		imageRegion.bufferRowLength = 0; // Row length of data to calculate data spacing
		imageRegion.bufferImageHeight = 0; // Image height to calculate data spacing
		imageRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Which aspect of image to copy
		imageRegion.imageSubresource.mipLevel = 0; // Mipmap level to copy
		imageRegion.imageSubresource.baseArrayLayer = 0; // Starting array layer (if array)
		imageRegion.imageSubresource.layerCount = 1; // Number of layers to copy starting at baseArrayLayer
		imageRegion.imageOffset = { 0,0,0 }; // Offset into image (as opposed to raw data on buffer offset)
		imageRegion.imageExtent = { width, height, 1 }; // Size of region to copy as (x,y,z) values

		// Copy buffer to given image
		vkCmdCopyBufferToImage(transferCommandBuffer, srcBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageRegion);

		endAndSubmitCommandBuffer(device, transferCommandPool, transferQueue, transferCommandBuffer);
	}

	static void transitionImageLayout(VkDevice device, VkQueue queue, VkCommandPool commandPool, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		// Create the buffer
		VkCommandBuffer commandBuffer = beginCommandBuffer(device, commandPool);

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout = oldLayout; // Layout to transition from
		imageMemoryBarrier.newLayout = newLayout; // Layout to transition to
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Queue family to transition from
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; // Queue family to transition to
		imageMemoryBarrier.image = image; // Image being accessed and modified as part of the barrier
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Aspect of image being altered
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0; // First mip level to start alterations on
		imageMemoryBarrier.subresourceRange.levelCount = 1; // Number of mip levels to alter stargin from baseMipLevel
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0; // First layer to start alterations on
		imageMemoryBarrier.subresourceRange.layerCount = 1; // Number of layers to alter starting from baseArrayLayer

		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;

		// If transition from new image to image ready to receive data...
		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = 0; // Memory access stage transition must after...
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;// Memory access stage transition must before...

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		// If transition from transfer destination to shader readable...
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			srcStage, dstStage, // Pipeline stages (mat to src and dst AccessMasks)
			0, // Dependency flags
			0, nullptr, // Memory barrier count + data
			0, nullptr, // Buffer memory barrier count + data
			1, &imageMemoryBarrier // Image Memory barrier count + data
		);

		endAndSubmitCommandBuffer(device, commandPool, queue, commandBuffer);
	}
}