#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(const char* shaderFolder, int winWidth, int winHeight, int samplesNum = 8);

	void Bind();

	void Update(int width, int height);

	void SetGammaValue(float gammaValue);
	float GetGammaValue();

private:
	Shader mShader;
	VertexArray mVertexArray;
	VertexBuffer mVertexBuffer;

	unsigned int mSampleID;
	unsigned int mPostProcessingID;
	unsigned int mPostProcessingTextureID;

	float mGammaValue;
};

