#pragma once

#include <glm/gtc/matrix_transform.hpp>

// To be implemented
enum RotationOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

class Transform
{
public:

	Transform();
	Transform(const glm::mat4& matrix);

	~Transform();

	const glm::vec3& GetPosition() const;
	const glm::vec3& GetOrientation() const;
	const glm::vec3& GetScale() const;

	void SetPosition(const glm::vec3& pos);
	void SetOrientation(const glm::vec3& orientation);
	void SetOrientation(const glm::vec3& axis, const float& angle);
	void SetOrientation(const glm::mat4& rotationMatrix);

	void ResetScale();

	void Translation(const glm::vec3& translation);
	void Rotate(const glm::vec3& rotation);
	void Scale(const glm::vec3& scale);

	const glm::mat4& GetMatrix();

private:

	glm::mat4 mMatrix{ 1.0f };
	bool mHasTransformed = false;

	glm::vec3 mPosition{ 0.0f };
	glm::mat4 mTranslation{ 1.0f };


	RotationOrder mRotationOrder = RotationOrder::XYZ;
	glm::vec3 mOrientation{ 0.0f };
	glm::mat4 mRotation{ 1.0f };

	glm::vec3 mScale{ 1.0f };
	glm::mat4 mScaling{ 1.0f };

};