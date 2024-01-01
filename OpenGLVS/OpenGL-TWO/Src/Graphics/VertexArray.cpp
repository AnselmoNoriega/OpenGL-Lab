#include "VertexArray.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_id);
}

void VertexArray::LinkVertexBuffer(VertexBuffer& vertexBuffer, const unsigned int layout)
{
	vertexBuffer.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
