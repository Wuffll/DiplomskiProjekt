#include "IndexBuffer.h"

#include <GL/glew.h>

#include "Debug.h"

#define INITIAL_BUFFER_SIZE 4 * 1024 * 1024; // 4 in bytes

unsigned int IndexBuffer::boundIBO = 0;

IndexBuffer::IndexBuffer()
	:
	mUsage(GL_STATIC_DRAW),
	mCount(0)
{
	glGenBuffers(1, &mRendererID);

	Debug::Print("IndexBuffer created without any data! (mRendererID = " + STRING(mRendererID) + ")");
}

IndexBuffer::IndexBuffer(const void* data, const unsigned int& count, const unsigned int& usage)
	:
	mUsage(GL_STATIC_DRAW),
	mCount(count)
{
	static_assert(sizeof(GLenum) == sizeof(unsigned int) && sizeof(GLuint) == sizeof(unsigned int));

	glGenBuffers(1, &mRendererID);
	FillBuffer(data, count, usage);
}

IndexBuffer::~IndexBuffer()
{
	Debug::Print("Index buffer " + STRING(mRendererID) + " destroyed!");
	glDeleteBuffers(1, &mRendererID);
}

void IndexBuffer::FillBuffer(const void* data, const unsigned int& count, const unsigned int& usage)
{
	AdjustBufferSize(count * sizeof(unsigned int), usage);

	InsertDataWithOffset(data, count, 0);

	mBufferSize = count * sizeof(unsigned int);

	mCount = count;
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Pointer to data</param>
/// <param name="count">Number of indices at the data pointer</param>
/// <param name="offset">Offset from where to buffer data; in bytes</param>
/// 
void IndexBuffer::InsertDataWithOffset(const void* data, const unsigned int& count, const unsigned int& offset)
{
	size_t size = count * sizeof(unsigned int);

	Bind();
	AdjustBufferSize(offset + size, mUsage);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);

	mBufferSize = (offset + size > mBufferSize) ? offset + size : mBufferSize;
	mCount = ((offset / sizeof(unsigned int)) + count > mCount) ? (offset / sizeof(unsigned int)) + count : mCount;
}

unsigned int IndexBuffer::AppendData(const void* data, const unsigned int& count)
{
	auto offset = mBufferSize;

	unsigned int* numbers = (unsigned int*)data;

	for (unsigned int i = 0; i < count; i++)
	{
		printf("%u\n", *(numbers + i));
	}

	Bind();
	AdjustBufferSize(offset + count * sizeof(unsigned int), mUsage);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(unsigned int), data);

	mBufferSize += count * sizeof(unsigned int);

	mCount = mBufferSize / sizeof(unsigned int);

	return offset;
}

/// <summary>
/// 
/// </summary>
/// <param name="newSize">In bytes</param>
/// <param name="usage">Buffer usage type</param>
/// 
void IndexBuffer::AdjustBufferSize(const unsigned int& newSize, const unsigned int& usage)
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
			data = malloc(mBufferSize);
			glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mBufferSize, data);
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBufferCapacity, data, usage);

		delete data;
	}
}

void IndexBuffer::Bind() const
{
	if (mCount == 0)
	{
		Debug::Print("Index buffer " + STRING(mRendererID) + " is not initialized! (count = 0)");
	}

	/*
	if (mRendererID == boundIBO)
		return;
	*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	boundIBO = mRendererID;
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	boundIBO = 0;
}

const unsigned int& IndexBuffer::GetIndicesCount() const
{
	return mCount;
}

const unsigned int& IndexBuffer::GetBufferSize() const
{
	return mBufferSize;
}

/// <summary>
/// 
/// </summary>
/// <returns>Returns offset (in bytes) at which the free memory starts</returns>
/// 
const unsigned int& IndexBuffer::GetOffset() const
{
	return mBufferSize;
}
