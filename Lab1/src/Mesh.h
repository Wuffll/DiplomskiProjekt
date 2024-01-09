#pragma once
#include <vector>
#include <string>
#include <limits>

#include "VertexArray.h"
#include "Vertex.h"

#include "Transform.h"

struct MinMax
{
	double min = std::numeric_limits<double>::max();
	double max = std::numeric_limits<double>::min();
};

class Mesh
{
public:

	Mesh(const std::string& filePath);
	~Mesh();

	const VertexBuffer& GetVB() const;
	const IndexBuffer& GetIB() const;

	const std::vector<Vertex> GetVertices() const;
	const std::vector<unsigned int> GetIndices() const;

	const Transform& GetTransform() const;
	
private:

	VertexBuffer mVBO;
	IndexBuffer mIBO;

	Transform mTransformMatrix;

	std::vector<Vertex> mVertices{};
	std::vector<unsigned int> mIndices{};
	std::vector<MinMax> mBoundingBox{};
	const std::string mFilePath;

};