#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <set>

#include "Utilities.h"

class VulkanRenderer
{
public:
	int init(GLFWwindow* newWindow);
	void cleanup();
private:
	GLFWwindow* window;

	// Vulkan Components
	VkInstance instance;
	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	VkSurfaceKHR surface;

	// Vulkan Functions
	// Create Functions
	void createInstance();
	void createLogicalDevice();
	void createSurface();

	// Get Functions
	void getPhysicalDevice();

	// Support Functions
	// Checking
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);

	// Validation section added by the community at Udemy
	// MARCO: What do you think?
#ifdef VK_DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif
	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};
	bool checkValidationLayerSupport();

	// Getting functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);
};

