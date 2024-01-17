#pragma once

#include <array>
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

enum BufferUsage
{
	STATIC = 0,
	DYNAMIC = 1,
	STREAM = 2
};

struct BufferInfoGroup
{
public:



private:

	VertexArray mVAO;
	std::vector<VertexBuffer> mVBOs;
	IndexBuffer mIBO;

	friend class BufferManagementSystem;
};

class BufferManagementSystem
{
public:

	BufferManagementSystem();
	~BufferManagementSystem();

	// const BufferInfoGroup& GenerateBuffers(const VertexBufferLayout& layout, const bool& buffersSeparated);

private:

	std::array<std::vector<BufferInfoGroup>, 3> mGroups;

};