#pragma once
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();

	void LinkVertexBuffer(VertexBuffer& vertexBuffer, const unsigned int layout);
	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int _id;
};

