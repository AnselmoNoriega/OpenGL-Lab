#pragma once
#include <vector>

#include "Shader.h"
#include "Model.h"

class Camera;

class ObjectGroup
{
public:
	ObjectGroup(const char* shaderFolder);
	~ObjectGroup();

	void ShadowMapUpdate(Camera& camera, unsigned int winWidth, unsigned int winHeight);
	void Update(Camera& camera);

	void AddModel
	(
		const char* file, const char* folder,
		unsigned int instances = 1,
		std::vector<glm::mat4> instanceMatrix = {}
	);

	void AddFlatModel
	(
		const char* file, const char* folder,
		unsigned int instances = 1,
		std::vector<glm::mat4> instanceMatrix = {}
	);

private:
	Mesh LightInit();

private:
	Mesh mLightObj;
	Shader mLightShader;
	VertexArray mLightVA;
	glm::mat4 mLightModel;

	Shader mShader;
	Shader mShadowMapShader;
	std::vector<Model> mModels;
	std::vector<Model> mFlatModels;

	glm::mat4 lightProj;
	unsigned int mShadowMap;
	unsigned int mShadowMapTex;

	unsigned int mShadowMapWidth;
	unsigned int mShadowMapHeight;
};

