#pragma once

#include <GL/glew.h>

class VertexBuffer
{
public:
	
	VertexBuffer();
	VertexBuffer(const void* data, const unsigned int& size, unsigned int usage);
	~VertexBuffer();

	void FillBuffer(const void* data, const unsigned int& size, unsigned int usage);
	void InsertDataWithOffset(const void* data, const unsigned int& size, const unsigned int& offset);
	unsigned int AppendData(const void* data, const unsigned int& size);

	void AdjustBufferSize(const unsigned int& newSize, const unsigned int& usage);

	const bool& IsInitialized() const;

	const unsigned int& GetRendererID() const;
	const unsigned int& GetBufferCapacity() const;
	const unsigned int& GetBufferSize() const;
	const unsigned int& GetOffset() const;

	void Bind() const;

	template <typename T>
	void Bind(const unsigned int& bindingIndex) const
	{
		glBindVertexBuffer(bindingIndex, mRendererID, 0, sizeof(T));
	}

	void Unbind() const;

private:

	static unsigned int boundVBO;

	bool mInitialized = false;
	unsigned int mRendererID = 0;
	unsigned int mUsage;

	unsigned int mBufferCapacity = 0; // (filled memory + reserved memory); in bytes
	unsigned int mBufferSize = 0; // (filled memory); in bytes
};