#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(const char* vertexShader, const char* fragmentShader, int winWidth, int winHeight, int samplesNum = 8);

	void Bind();

	void Update(int width, int height);

private:
	Shader mShader;
	VertexArray mVertexArray;
	VertexBuffer mVertexBuffer;

	unsigned int mSampleID;
	unsigned int mPostProcessingID;
	unsigned int mPostProcessingTextureID;
};

