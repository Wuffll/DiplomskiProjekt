#include "Renderer.h"

Renderer::Renderer(Shader& shader)
	:
	mShader(shader)
{
}

void Renderer::Draw() const
{
	mShader.Bind();

	for (const auto& obj : mDrawableObjects)
	{
		if (obj->IsActive())
		{
			mShader.SetUniformMatrix4f("model", obj->GetTransform().GetMatrix());
			obj->Draw();
		}
	}
}

void Renderer::AddDrawableObject(Drawable& object)
{
	mDrawableObjects.push_back(&object);
}
