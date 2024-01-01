#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const float* vertices, const unsigned int size);

	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int _id;
};

