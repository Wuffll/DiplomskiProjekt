#include "Transform.h"

Transform::Transform()
{
}

Transform::Transform(const glm::mat4& matrix)
	:
	mMatrix(matrix)
{
}

Transform::~Transform()
{
}

const glm::vec3& Transform::GetPosition() const
{
	return mPosition;
}

const glm::vec3& Transform::GetOrientation() const
{
	return mOrientation;
}

const glm::vec3& Transform::GetScale() const
{
	return mScale;
}

void Transform::SetPosition(const glm::vec3& pos)
{
	mHasTransformed = true;

	glm::vec3 direction = (pos - mPosition);
	mTranslation = glm::translate(mTranslation, direction);
	mPosition = pos;
}

void Transform::SetOrientation(const glm::vec3& orientation)
{
	mHasTransformed = true;

	glm::vec3 endOrientation = orientation - mOrientation;

	glm::vec3 axis = { 1.0f, 0.0f, 0.0f };
	mRotation = glm::rotate(mRotation, glm::radians(endOrientation.x), axis);
	
	axis = { 0.0f, 1.0f, 0.0f };
	mRotation = glm::rotate(mRotation, glm::radians(endOrientation.y), axis);
	
	axis = { 0.0f, 0.0f, 1.0f };
	mRotation = glm::rotate(mRotation, glm::radians(endOrientation.z), axis);

	mOrientation = endOrientation;
}

void Transform::SetOrientation(const glm::vec3& axis, const float& angle)
{
	mHasTransformed = true;

	mRotation = glm::mat4(1.0f);

	mRotation = glm::rotate(mRotation, angle, axis);
}

void Transform::SetOrientation(const glm::mat4& rotationMatrix)
{
	mHasTransformed = true;

	mRotation = rotationMatrix;

	printf("Matrix set orientation used!\n");
}

// not sure if this actually works; gotta test
void Transform::ResetScale()
{
	mHasTransformed = true;

	mScale = { 1.0f, 1.0f, 1.0f };
	mScaling = { glm::mat4{1.0f} };
}

void Transform::Translation(const glm::vec3& translation)
{
	mHasTransformed = true;

	mTranslation = glm::translate(mTranslation, translation);
	mPosition += translation;
}

void Transform::Rotate(const glm::vec3& rotation)
{
	mHasTransformed = true;

	glm::vec3 axis{ 1.0f, 0.0f, 0.0f };
	mRotation = glm::rotate(mRotation, glm::radians(rotation.x), axis);

	axis = { 0.0f, 1.0f, 0.0f };
	mRotation = glm::rotate(mRotation, glm::radians(rotation.y), axis);

	axis = { 0.0f, 0.0f, 1.0f };
	mRotation = glm::rotate(mRotation, glm::radians(rotation.z), axis);

	mOrientation += rotation;
}

void Transform::Scale(const glm::vec3& scale)
{
	mHasTransformed = true;

	mScaling = glm::scale(mScaling, scale);
	mScale *= scale;
}

const glm::mat4& Transform::GetMatrix()
{
	if(!mHasTransformed)
		return mMatrix;

	mMatrix = mTranslation * mRotation * mScaling;

	mHasTransformed = false;

	return mMatrix;
}

glm::mat4 Transform::aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
	{
		glm::mat4 to;

		to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
		to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
		to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
		to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;

		return to;
	}
}
