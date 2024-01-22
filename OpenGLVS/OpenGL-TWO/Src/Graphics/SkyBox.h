#pragma once
#include "Shader.h"
#include "VertexArray.h"

class Camera;

class SkyBox
{
public:
	SkyBox(const char* vertexShader, const char* fragmentShader, int screenWidth, int screenHeight);
	~SkyBox();

	void Update(Camera& camera);

private:
	Shader mShader;
	VertexArray mVertexArray;

	unsigned int mTextureID;

	unsigned int mScreenWidth;
	unsigned int mScreenHeight;
};

