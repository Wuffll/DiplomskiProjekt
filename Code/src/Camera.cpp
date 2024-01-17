#include "Camera.h"

#include "Debug.h"

Camera::Camera()
	:
	mView(glm::lookAt(mPosition, mPosition + mFront, mUp)),
	mShaderUniformName("view")
{
	Debug::Print("Camera default constructor (Shader* mShader not set!)");
}

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, Shader* shader)
	:
	mPosition(position),
	mFront(front),
	mUp(up),
	mView(glm::lookAt(position, position + front, up)),
	mShader(shader)
{
}

void Camera::SetMoveSpeed(const float& newSpeed)
{
	mMoveSpeed = newSpeed;
}

void Camera::SetShader(const std::string& uniformName, Shader* shader)
{
	mShader = shader;
	mShaderUniformName = uniformName;

	UpdateShaderUniform();
}

Transform& Camera::GetView()
{
	return mView;
}

void Camera::Rotate(const glm::vec3& rotation)
{
	mView.Rotate(rotation);

	UpdateShaderUniform();
}

void Camera::SetOrientation(const glm::vec3& rotation)
{
	mView.SetOrientation(rotation);

	UpdateShaderUniform();
}

void Camera::SetPosition(const glm::vec3& position)
{
	mView.SetPosition(position);

	UpdateShaderUniform();
}

void Camera::Move(const glm::vec3& direction)
{
	mView.Translation(direction);

	UpdateShaderUniform();
}

void Camera::ResetTransform()
{
	mView = glm::lookAt(mPosition, mPosition + mFront, mUp);

	UpdateShaderUniform();
}

void Camera::UpdateShaderUniform()
{
	if (mShader != nullptr)
		mShader->SetUniformMatrix4f(mShaderUniformName, mView.GetMatrix());
	else
		Debug::ThrowException("Shader not set for Camera!");
}
