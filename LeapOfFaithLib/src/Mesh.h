#pragma once
#include <glm/mat4x4.hpp>

struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;
struct VkDevice_T;
typedef VkDevice_T* VkDevice;
struct VkQueue_T;
typedef VkQueue_T* VkQueue;
struct VkCommandPool_T;
typedef VkCommandPool_T* VkCommandPool;

#include <vector>

#include "Utilities.h"

struct Model {
	glm::mat4 model;
};

class Mesh
{
public:
	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, 
		VkQueue transferQueue, VkCommandPool transferCommandPool, 
		std::vector<Vertex>* vertices, std::vector<uint32_t>* indices,
		int newTexId);

	void setModel(glm::mat4 newModel);
	inline Model getModel() const { return model; }

	inline int getTexId() const { return texId; }

	inline int getVertexCount() const { return vertexCount; }
	VkBuffer getVertexBuffer() const;

	inline int getIndexCount() const { return indexCount; }
	VkBuffer getIndexBuffer() const;
	
	void destroyBuffers();
private:
	Model model;
	int texId;

	int vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	int indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);
};