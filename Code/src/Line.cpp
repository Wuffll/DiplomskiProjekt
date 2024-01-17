#include "Line.h"

unsigned int Line::mLineIdentifierCounter = 0;

Line::Line()
	:
	mLineID(mLineIdentifierCounter + 1)
{
	mLineIdentifierCounter++;
}

Line::Line(const glm::vec3& startPos, const glm::vec3& endPos)
	:
	mLineID(mLineIdentifierCounter + 1),
	mStartPoint(startPos),
	mEndPoint(endPos)
{
	mLineIdentifierCounter++;
}

Line::Line(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec3& color)
	:
	mLineID(mLineIdentifierCounter + 1),
	mStartPoint(startPos),
	mEndPoint(endPos),
	mColor(color)
{
	mLineIdentifierCounter++;
}

void Line::SetStartPoint(const glm::vec3& point)
{
	mStartPoint = point;
}

void Line::SetEndPoint(const glm::vec3& point)
{
	mEndPoint = point;
}

void Line::SetColor(const glm::vec3& color)
{
	mColor = color;
}
