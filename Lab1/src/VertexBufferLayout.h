#pragma once

#include <GL/glew.h>

#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int SizeOfDataType(GLenum type)
	{
		switch (type)
		{
			case GL_DOUBLE:				return sizeof(double);
			case GL_INT:				return sizeof(int);
			case GL_FLOAT:				return sizeof(float);
			case GL_UNSIGNED_INT:		return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE:		return sizeof(unsigned char);
		}

		return 0;
	}

	bool operator==(const VertexBufferElement& rhs)
	{
		return type == rhs.type && count == rhs.count && normalized == rhs.normalized;
	}
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() = default;

	const std::vector<VertexBufferElement>& GetElements() const;
	const unsigned int& GetStride() const;

	const bool IsInitialized() const;

	bool operator==(const VertexBufferLayout& rhs)
	{
		size_t size = mElements.size();
		if (size != rhs.mElements.size())
			return false;

		for (size_t i = 0; i < size; i++)
		{
			if (mElements[i] != rhs.mElements[i])
				return false;
		}

		return true;
	}

	template<typename T>
	void Push(const unsigned int& count)
	{
		printf("Tried using unsupported data type!(%s)\n", typeid(T).name());
		exit(-1);
	}

	template<>
	void Push<float>(const unsigned int& count)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE });
		mStride += count * VertexBufferElement::SizeOfDataType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(const unsigned int& count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		mStride += count * VertexBufferElement::SizeOfDataType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(const unsigned int& count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += count * VertexBufferElement::SizeOfDataType(GL_UNSIGNED_BYTE);
	}

private:

	std::vector<VertexBufferElement> mElements{};
	unsigned int mStride = 0;

};