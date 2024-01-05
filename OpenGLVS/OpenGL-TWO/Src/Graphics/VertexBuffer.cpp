#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &mID);
	glBindBuffer(GL_ARRAY_BUFFER, mID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
