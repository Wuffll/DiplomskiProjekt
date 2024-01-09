#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "Drawable.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"

class Objekt : public Drawable
{
public:

	Objekt(const std::string& name, const std::string& meshFilePath, Shader& shader);
	~Objekt();

	void Draw();

	const bool& IsActive() const;
	virtual void SetActive(const bool& value);
	static void ToggleActive(Objekt& obj);

	Transform& GetTransform();

private:

	std::string mName;
	bool mActive = true;
	Mesh mMesh;
	Transform mTransform;
	VertexArray mVAO;
	Shader& mShader;

};