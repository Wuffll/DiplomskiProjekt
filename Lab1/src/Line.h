#pragma once

#include <glm/glm.hpp>

class Line
{
public:

	Line();
	Line(const glm::vec3& startPos, const glm::vec3& endPos);
	Line(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec3& color);

	void SetStartPoint(const glm::vec3& point);
	void SetEndPoint(const glm::vec3& point);
	void SetColor(const glm::vec3& color);

private:

	static unsigned int mLineIdentifierCounter;

	unsigned int mLineID;

	glm::vec3 mStartPoint{ 0.0f };
	glm::vec3 mEndPoint{ 0.0f };
	glm::vec3 mColor{ 1.0f };


};