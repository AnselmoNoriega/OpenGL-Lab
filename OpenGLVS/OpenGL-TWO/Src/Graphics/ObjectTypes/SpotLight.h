#pragma once
#define GLFW_INCLUDE_NONE
#include "Shader.h"
#include <glm/gtx/rotate_vector.hpp>

class Model;
class Camera;

class SpotLight
{
public:
	SpotLight();

	void Initialize
	(
		unsigned int shadowMapWidth,
		unsigned int shadowMapHeight, 
		float farplane, 
		glm::vec3 lightPos
	);

	void BindTexture();

	unsigned int GetFrameBuffer() { return mFrameBufferID; }
	Shader& GetShader() { return mShader; }

private:
	Shader mShader;
	unsigned int mFrameBufferID;
	unsigned int mTextureID;
};

