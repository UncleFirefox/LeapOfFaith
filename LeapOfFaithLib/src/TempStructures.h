#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

// Forward declaration
struct aiScene;

struct ImportResult
{
	std::vector<std::string> textureNames;
	const aiScene* scene;
};

struct MeshBuffer
{
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};
