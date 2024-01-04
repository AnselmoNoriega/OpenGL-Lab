#pragma once

class ElementBuffer
{
public:
	ElementBuffer(unsigned int* indices, unsigned int size);

	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int mID;
};

