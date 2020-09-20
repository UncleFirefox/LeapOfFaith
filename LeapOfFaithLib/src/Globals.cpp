#include "Globals.h"

namespace Globals {
	extern MainDevice* mainDevice = nullptr;

	extern VkQueue* graphicsQueue = nullptr;
	extern VkCommandPool* graphicsCommandPool = nullptr;

	extern std::vector<VkImage>* textureImages = nullptr;
	extern std::vector<VkImageView>* textureImageViews = nullptr;
	extern std::vector<VkDeviceMemory>* textureImageMemory = nullptr;

	extern VkDescriptorPool* samplerDescriptorPool = nullptr;
	extern VkDescriptorSetLayout* samplerSetLayout = nullptr;
	extern VkSampler* textureSampler = nullptr;
	extern std::vector<VkDescriptorSet>* samplerDescriptorSets = nullptr;
}