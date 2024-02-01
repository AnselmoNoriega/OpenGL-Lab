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

	void Update(Camera& camera);

	void AddModel(const char* file, const char* folder = "");
	void AddFlatModel(const char* file, const char* folder = "");

private:
	Shader mShader;
	std::vector<Model> mModels;
	std::vector<Model> mFlatModels;
};

