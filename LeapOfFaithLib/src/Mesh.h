#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, int newTexId);

	void setModel(glm::mat4 newModel);
	glm::mat4 getModel();

	int getTexId();
	int getVertexCount();
	int getIndexCount();
private:
	glm::mat4 model;
	std::vector<Vertex>* vertices;
	std::vector<uint32_t>* indices;
	int vertexCount;
	int indexCount;
	int texId;
};