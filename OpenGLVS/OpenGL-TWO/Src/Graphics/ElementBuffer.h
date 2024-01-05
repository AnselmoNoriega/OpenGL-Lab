#pragma once
#include <vector>

class ElementBuffer
{
public:
	ElementBuffer(std::vector<unsigned int>& indices);
	~ElementBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int mID;
};

