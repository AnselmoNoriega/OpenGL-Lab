#pragma once
#include <vector>

#include "Shader.h"
#include "Model.h"

class Camera;

class ObjectGroup
{
public:
	ObjectGroup(const char* vertexShader, const char* fragmentShader);
	~ObjectGroup();

	void Update(Camera& camera);

	void AddModel(const char* file, const char* folder = "");


private:
	Shader mShader;
	std::vector<Model> mModels;
};

