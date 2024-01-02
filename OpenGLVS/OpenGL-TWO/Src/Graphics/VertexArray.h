#pragma once
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();

	void LinkVertexBuffer(VertexBuffer& vertexBuffer, unsigned int layout, unsigned int vertSize, unsigned int stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int _id;
};

