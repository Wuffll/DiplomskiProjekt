#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;

	// Gets average of normals
	Vertex AddPosition(const Vertex& other)
	{
		glm::vec3 newPos = pos + other.pos;
		glm::vec3 newNormal = (normal + other.normal) / 2.0f;
		return {newPos, newNormal};
	}
};