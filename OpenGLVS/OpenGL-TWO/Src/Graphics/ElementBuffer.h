#pragma once

class ElementBuffer
{
public:
	ElementBuffer(unsigned int* indices, unsigned int size);
	~ElementBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int mID;
};

