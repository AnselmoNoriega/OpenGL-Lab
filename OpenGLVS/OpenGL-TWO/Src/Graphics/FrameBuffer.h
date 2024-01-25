#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(const char* vertexShader, const char* fragmentShader, int winWidth, int winHeight);

	void Bind();

	void Update();

private:
	Shader mShader;
	VertexArray mVertexArray;
	VertexBuffer mVertexBuffer;
	Texture mTexure;

	unsigned int mID;
};

