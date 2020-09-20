#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Mesh.h"

struct aiScene;
struct aiNode;
struct aiMesh;
typedef unsigned char stbi_uc;

class MeshModel
{
public:
	MeshModel();

	void Load(const std::string& modelFile);

	size_t getMeshCount();
	Mesh* getMesh(size_t index);

	glm::mat4 getModel();
	void setModel(glm::mat4 newModel);

	void destroyMeshModel();

	static std::vector<std::string> LoadMaterials(const aiScene* scene);
	static int createTexture(const std::string& fileName);
	static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	static VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags,
		VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);

	~MeshModel();
private:
	static std::vector<Mesh> LoadNode(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		aiNode* node, const aiScene* scene, std::vector<int> matToTex);
	static Mesh LoadMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		aiMesh* mesh, const aiScene* scene, std::vector<int> matToTex);

	static int createTextureImage(const std::string& fileName);
	static int createTextureDescriptor(VkImageView textureImage);
	static stbi_uc* loadTextureFile(const std::string& fileName, int* width, int* height, VkDeviceSize* imageSize);
	std::vector<Mesh> meshList;
	glm::mat4 model;
};