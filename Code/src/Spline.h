#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Drawable.h"
#include "Mesh.h"
#include "Transform.h"

class CubicBSpline : public Drawable
{
public:

	CubicBSpline(std::vector<glm::vec3>& controlPoints, const unsigned int& sampleRate = 1000);
	~CubicBSpline();

	const std::vector<Vertex>& GetSplinePoints() const;
	const std::vector<glm::mat4>& GetRotationMatrices() const;

	void FillSplinePoints(std::vector<glm::vec3>& controlPoints, const unsigned int& sampleRate);

	void Draw();
	virtual const bool& IsActive() const;
	virtual void SetActive(const bool& value);

	Transform& GetTransform();
	const std::vector<Vertex>& GetTangents() const;

private:

	bool mBoolActive = true;
	int mActive = 0;
	VertexArray mVArray;
	VertexBuffer mVBuffer;
	VertexBuffer mVBufferGuides;
	IndexBuffer mIBuffer;
	Transform mTransform;
	std::vector<glm::vec3> mControlPoints;
	std::vector<Vertex> mSplinePoints;
	std::vector<Vertex> mTangents;
	std::vector<Vertex> mNormals;
	std::vector<Vertex> mBinormals;
	std::vector<Vertex> mCurrentGuides;
	std::vector<glm::mat4> mRotationMatrices;
	int mNumOfSegments;
	unsigned int mSampleRate;

};