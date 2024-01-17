#pragma once

class IndexBuffer
{
public:

	IndexBuffer();
	IndexBuffer(const void* data, const unsigned int& count, const unsigned int& usage);
	~IndexBuffer();

	void FillBuffer(const void* data, const unsigned int& count, const unsigned int& usage);
	void InsertDataWithOffset(const void* data, const unsigned int& count, const unsigned int& offset);
	unsigned int AppendData(const void* data, const unsigned int& count);

	void AdjustBufferSize(const unsigned int& newSize, const unsigned int& usage);

	void Bind() const;
	void Unbind() const;

	const unsigned int& GetIndicesCount() const;
	const unsigned int& GetBufferSize() const;
	const unsigned int& GetOffset() const;


private:

	static unsigned int boundIBO;

	unsigned int mRendererID = 0;
	unsigned int mUsage;

	unsigned int mCount = 0;
	unsigned int mBufferCapacity = 0; // free space + used up space (= initial size); in bytes
	unsigned int mBufferSize = 0; // used up space; in bytes
};