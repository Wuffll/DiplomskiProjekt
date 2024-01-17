#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Transform.h"
#include "Shader.h"

class Camera
{
public:

	Camera();
	Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, Shader* shader = nullptr);

	void SetMoveSpeed(const float& newSpeed);
	void SetShader(const std::string& uniformName, Shader* shader);

	Transform& GetView();

	void Rotate(const glm::vec3& rotation);
	void SetOrientation(const glm::vec3& rotation);

	void SetPosition(const glm::vec3& position);
	void Move(const glm::vec3& direction);

	void ResetTransform();

private:

	void UpdateShaderUniform();

	glm::vec3 mPosition = { 0.0f, 0.0f, 1.0f };
	glm::vec3 mFront = { 0.0f, 0.0f, -1.0f };
	glm::vec3 mUp = { 0.0f, 1.0f, 0.0f };

	float mMoveSpeed = 5.0f;

	Transform mView;

	Shader* mShader;
	std::string mShaderUniformName;

};