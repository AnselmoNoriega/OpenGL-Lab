#include "ObjectGroup.h"
#include "UniformHandler.h"

ObjectGroup::ObjectGroup(const char* vertexShader, const char* fragmentShader):
	mShader(vertexShader, fragmentShader)
{
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	mShader.UseProgram();
	glUniform4f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
}

void ObjectGroup::Update(Camera& camera)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Update(mShader, camera);
	}
	glDisable(GL_CULL_FACE);
}

void ObjectGroup::AddModel(const char* file, const char* folder)
{
	mModels.emplace_back(Model(file, folder));
}

ObjectGroup::~ObjectGroup()
{
	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Delete();
	}
}
