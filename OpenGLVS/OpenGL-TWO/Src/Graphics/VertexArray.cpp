#include "VertexArray.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_id);
}

void VertexArray::LinkVertexBuffer(VertexBuffer& vertexBuffer, unsigned int layout, unsigned int vertSize, unsigned int stride, void* offset)
{
	vertexBuffer.Bind();
	glVertexAttribPointer(layout, vertSize, GL_FLOAT, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vertexBuffer.Unbind();
}

void VertexArray::Bind()
{
	glBindVertexArray(_id);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &_id);
}
