#pragma once
#include <string>
#include "Renderer.h"

class Texture
{
private:
	unsigned int mRendererID;
	std::string mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return mWidth; }
	inline int GetmHeight() const { return mHeight; }
};

