#pragma once

#include<vector>

#include "VertexArray.h"
#include "Shader.h"
#include "Drawable.h"

class Renderer
{
public:

	Renderer(Shader& shader);
	~Renderer() = default;

	void Draw() const;

	void AddDrawableObject(Drawable& object);

private:

	std::vector<Drawable*> mDrawableObjects;
	Shader& mShader; // temporary; should be assigned for each mesh (/poly)?

};