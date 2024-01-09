#include "VertexArray.h"

#include "Debug.h"

unsigned int VertexArray::ActiveVAO = 0;

VertexArray::VertexArray()
	:
	mUsage(GL_STATIC_DRAW),
	mDrawingMode(GL_TRIANGLES)
{
	glGenVertexArrays(1, &mRendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &mRendererID);
}

const VertexBufferLayout& VertexArray::GetLayout() const
{
	return mLayout;
}

const unsigned int& VertexArray::GetUsage() const
{
	return mUsage;
}

const unsigned int& VertexArray::GetDrawingMode() const
{
	return mDrawingMode;
}

void VertexArray::SetLayout(const VertexBufferLayout& layout, const bool& buffersSeperated)
{
	mLayout = layout;
	mLayoutBuffersSeperated = buffersSeperated;
}

void VertexArray::SetUsage(const unsigned int& usageType)
{
	mUsage = usageType;
}

void VertexArray::SetDrawingMode(const unsigned int& drawingMode)
{
	mDrawingMode = drawingMode;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib)
{
	if (!mLayout.IsInitialized())
	{
		Debug::ThrowException("Layout is not set for Vertex Array! (mRendererID = " + STRING(mRendererID) + ")");
	}

	Bind();

	// Don't forget to bind vb's to correct binding indices before drawing !!!
	AssignVertexAttributes();

	ib.Bind();
}

void VertexArray::Bind() const
{
	if (mRendererID == 0)
		Debug::ThrowException("ERROR: VertexArray not initialized !!!");

	if(mRendererID != ActiveVAO)
		glBindVertexArray(mRendererID);

	ActiveVAO = mRendererID;
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AssignVertexAttributes()
{
	unsigned int offset = 0;
	const auto& elements = mLayout.GetElements();

	unsigned int bindingIndex = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& e = elements[i];

		glEnableVertexAttribArray(i);
		// glVertexAttribPointer(i, e.count, e.type, e.normalized, mLayout.GetStride(), (void*)(offset));
		glVertexAttribFormat(i, e.count, e.type, e.normalized, (mLayoutBuffersSeperated) ? 0 : offset);
		glVertexAttribBinding(i, bindingIndex);

		if (mLayoutBuffersSeperated) 
			bindingIndex++;

		offset += e.count * VertexBufferElement::SizeOfDataType(e.type);
	}
}
