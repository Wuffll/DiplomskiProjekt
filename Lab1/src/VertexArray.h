#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:

	VertexArray();
	~VertexArray();

	const VertexBufferLayout& GetLayout() const;
	const unsigned int& GetUsage() const;
	const unsigned int& GetDrawingMode() const;

	void SetLayout(const VertexBufferLayout& layout, const bool& buffersSeperated);
	void SetUsage(const unsigned int& usageType);
	void SetDrawingMode(const unsigned int& drawingMode);

	void AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;

private:

	void AssignVertexAttributes();

	static unsigned int ActiveVAO;

	unsigned int mRendererID;

	VertexBufferLayout mLayout;
	bool mLayoutBuffersSeperated = false;

	unsigned int mUsage;
	unsigned int mDrawingMode;

};