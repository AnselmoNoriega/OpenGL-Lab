#pragma once
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void LinkVertexBuffer(VertexBuffer& vertexBuffer, unsigned int layout, unsigned int vertSize, unsigned int stride, void* offset);
	void Bind();
	void Unbind();

private:
	unsigned int mID;
};

