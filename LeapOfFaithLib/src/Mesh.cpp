#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices,
	int newTexId)
{
	this->indices = indices;
	this->vertices = vertices;
	vertexCount = vertices->size();
	indexCount = indices->size();

	model = glm::mat4(1.0f);
	texId = newTexId;
}

void Mesh::setModel(glm::mat4 newModel)
{
	model = newModel;
}

int Mesh::getTexId()
{
	return texId;
}

int Mesh::getVertexCount()
{
	return vertexCount;
}

int Mesh::getIndexCount()
{
	return indexCount;
}
