#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const float* vertices, const unsigned int size);
	~VertexBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int mID;
};

