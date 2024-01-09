#include "VertexBuffer.h"

#include "Debug.h"
#include "Vertex.h"

#define INITIAL_BUFFER_SIZE 16 * 1024 * 1024 // 16 MB in bytes

unsigned int VertexBuffer::boundVBO = 0;

VertexBuffer::VertexBuffer()
	:
	mUsage(GL_STATIC_DRAW)
{
	glGenBuffers(1, &mRendererID);

	Debug::Print("VertexBuffer created without any data! (mRendererID = " + STRING(mRendererID) + ")");
}

VertexBuffer::VertexBuffer(const void* data, const unsigned int& size, unsigned int usage)
	:
	mInitialized(true),
	mUsage(usage)
{
	static_assert(sizeof(GLenum) == sizeof(unsigned int));

	glGenBuffers(1, &mRendererID);
	FillBuffer(data, size, usage);
}

VertexBuffer::~VertexBuffer()
{
	Debug::Print("Vertex buffer " + STRING(mRendererID) + " destroyed!");
	glDeleteBuffers(1, &mRendererID);
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Pointer to data being transfered</param>
/// <param name="size">Size of data being transfered; in bytes</param>
/// <param name="usage">Buffer Usage Type</param>
/// 
void VertexBuffer::FillBuffer(const void* data, const unsigned int& size, unsigned int usage)
{
	if (usage != mUsage)
		Debug::Print("Inserted usage type is different from the member usage type! (mRendererID = " + STRING(mRendererID) + ")");

	AdjustBufferSize(size, usage);

	InsertDataWithOffset(data, size, 0);

	mInitialized = true;
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Pointer to data being inserted</param>
/// <param name="size">Size of data; in bytes</param>
/// <param name="offset">Offset; in bytes</param>
/// 
void VertexBuffer::InsertDataWithOffset(const void* data, const unsigned int& size, const unsigned int& offset)
{
	Bind();
	AdjustBufferSize(offset + size, mUsage);

	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	mBufferSize = (offset + size > mBufferSize) ? offset + size : mBufferSize;
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Pointer to the data</param>
/// <param name="size">Size of data; in bytes</param>
/// <returns></returns>
/// 
unsigned int VertexBuffer::AppendData(const void* data, const unsigned int& size)
{
	auto offset = mBufferSize;

	Bind();
	AdjustBufferSize(offset + size, mUsage);

	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	mBufferSize += size;

	return offset;
}

/// <summary>
/// 
/// </summary>
/// <param name="newSize">In bytes</param>
/// <param name="usage">Buffer Usage Type</param>
/// 
void VertexBuffer::AdjustBufferSize(const unsigned int& newSize, const unsigned int& usage)
{
	bool sizeChanged = false;

	if (mBufferCapacity == 0)
	{
		mBufferCapacity = INITIAL_BUFFER_SIZE;
		sizeChanged = true;
	}

	while (mBufferCapacity < newSize)
	{
		mBufferCapacity += mBufferCapacity;
		sizeChanged = true;
	}

	if (sizeChanged)
	{
		Bind();

		void* data = nullptr;

		if (mBufferSize != 0)
		{
			data = new char[mBufferSize];
			glGetBufferSubData(GL_ARRAY_BUFFER, 0, mBufferSize, data);
		}

		glBufferData(GL_ARRAY_BUFFER, mBufferCapacity, data, usage);

		if (data)
			delete data;
	}
}

const bool& VertexBuffer::IsInitialized() const
{
	return mInitialized;
}

const unsigned int& VertexBuffer::GetRendererID() const
{
	return mRendererID;
}

const unsigned int& VertexBuffer::GetBufferCapacity() const
{
	return mBufferCapacity;
}

const unsigned int& VertexBuffer::GetBufferSize() const
{
	return mBufferSize;
}

/// <summary>
/// 
/// </summary>
/// <returns>Returns offset (in bytes) at which the free memory starts</returns>
/// 
const unsigned int& VertexBuffer::GetOffset() const
{
	return mBufferSize;
}

void VertexBuffer::Bind() const
{
	if (!mInitialized)
	{
		Debug::Print("Vertex buffer " + STRING(mRendererID) + " is uninitialized! (mInitialized = false)");
	}

	/*
	if (boundVBO == mRendererID)
		return;
	*/

	glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	boundVBO = mRendererID;
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	boundVBO = 0;
}
