#pragma once

class ElementBuffer
{
public:
	ElementBuffer(const unsigned int* indices, const unsigned int size);

	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int _id;
};

