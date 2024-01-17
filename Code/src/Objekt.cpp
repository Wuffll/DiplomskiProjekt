#include "Objekt.h"

Objekt::Objekt(const std::string& name, const std::string& meshFilePath, Shader& shader)
	:
	mName(name),
	mMesh(meshFilePath),
	mShader(shader),
	mTransform(mMesh.GetTransform())
{
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);

	mVAO.SetLayout(layout, false);
	mVAO.SetDrawingMode(GL_TRIANGLES);
	mVAO.SetUsage(GL_STATIC_DRAW);
	mVAO.AddBuffer(mMesh.GetVB(), mMesh.GetIB());
}

Objekt::~Objekt()
{
}

void Objekt::Draw()
{
	mShader.Bind();
	mVAO.Bind();
	mMesh.GetVB().Bind<Vertex>(0);
	mMesh.GetIB().Bind();

	glDrawElements(mVAO.GetDrawingMode(), mMesh.GetIB().GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
}

const bool& Objekt::IsActive() const
{
	return mActive;
}

void Objekt::SetActive(const bool& value)
{
	mActive = value;
}

void Objekt::ToggleActive(Objekt& obj)
{
	obj.mActive = !obj.mActive;
}

Transform& Objekt::GetTransform()
{
	return mTransform;
}
