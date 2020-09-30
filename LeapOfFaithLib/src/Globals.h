#pragma once

struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
struct VkDevice_T;
typedef VkDevice_T* VkDevice;

namespace Globals
{
	struct VkContext {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	};

	extern VkContext* vkContext;
}