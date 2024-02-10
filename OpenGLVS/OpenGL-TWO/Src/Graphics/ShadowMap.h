#pragma once

#include "Shader.h"
#include "ObjectTypes/SpotLight.h"
#include "Model.h"

#include <glm/gtx/rotate_vector.hpp>

class Camera;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Initialize(glm::vec3 lightPos);
	void Update(Camera& camera);
	void UpdateShadows(Model& model, Camera& camera);

	void BindTexture();

	glm::mat4& GetLightProj() { return lightProj; }

	unsigned int GetShadowMapWidth() { return mShadowMapWidth; }
	unsigned int GetShadowMapHeight() { return mShadowMapHeight; }
	unsigned int GetFrameBuffer() { return mSpotLight.GetFrameBuffer(); }

	float GetFarPlane() { return farPlane; }

private:
	Shader mShader;
	Shader mLightShader;

	SpotLight mSpotLight;

	Model mLightObj;

	glm::mat4 lightProj;

	unsigned int mShadowMap;
	unsigned int mShadowMapTex;

	unsigned int mShadowMapWidth;
	unsigned int mShadowMapHeight;

	float farPlane;
};

