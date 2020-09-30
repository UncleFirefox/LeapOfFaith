#pragma once

struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
struct VkDevice_T;
typedef VkDevice_T* VkDevice;
struct VkQueue_T;
typedef VkQueue_T* VkQueue;
struct VkCommandPool_T;
typedef VkCommandPool_T* VkCommandPool;

namespace Globals
{
	struct VkContext {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkQueue graphicsQueue;
		VkCommandPool graphicsCommandPool;
	};

	extern VkContext* vkContext;
}