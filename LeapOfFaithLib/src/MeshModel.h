#pragma once

#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include "Mesh.h"

typedef unsigned char stbi_uc;

struct aiScene;
struct aiNode;
struct aiMesh;

class MeshModel
{
public:
	MeshModel();
	void LoadFile(const std::string& modelFile, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& graphicsCommandPool,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	inline size_t getMeshCount() const { return meshList.size(); }
	Mesh* getMesh(size_t index);

	inline glm::mat4 getModel() const { return model; }
	void setModel(glm::mat4 newModel);

	void destroyMeshModel();

	static std::vector<std::string> LoadMaterials(const aiScene* scene);
	
	static stbi_uc* loadTextureFile(const std::string& fileName, int* width, int* height, VkDeviceSize* imageSize);
	static int createTexture(std::string fileName, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, VkQueue& graphicsQueue, VkCommandPool& graphicsCommandPool,
		std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory, std::vector<VkImageView>& textureImageViews,
		VkDescriptorPool& samplerDescriptorPool, VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);
	static int createTextureImage(const std::string& fileName, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice,
		VkQueue& graphicsQueue, VkCommandPool& graphicsCommandPool, std::vector<VkImage>& textureImages, std::vector<VkDeviceMemory>& textureImageMemory);

	static VkImage createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propFlags,
		VkDeviceMemory* imageMemory, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice);

	static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice& logicalDevice);

	static int createTextureDescriptor(VkImageView textureImage, VkDescriptorPool& samplerDescriptorPool, VkDevice& logicalDevice,
		VkDescriptorSetLayout& samplerSetLayout, VkSampler& textureSampler, std::vector<VkDescriptorSet>& samplerDescriptorSets);

	static std::vector<Mesh> LoadNode(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		aiNode* node, const aiScene* scene, std::vector<int> matToTex);
	static Mesh LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		aiMesh* mesh, const aiScene* scene, std::vector<int> matToTex);
private:
	std::vector<Mesh> meshList;
	glm::mat4 model;
};

