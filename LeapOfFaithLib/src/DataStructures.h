#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Vertex data representation
struct Vertex {
	glm::vec3 pos; // Vertex position (x,y,z)
	glm::vec3 col; // Vertex Color (r,g,b)
	glm::vec2 tex; // Texture coords (u,v)
};
