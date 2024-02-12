#include "ObjectGroup.h"
#include "ElementBuffer.h"
#include "UniformHandler.h"

ObjectGroup::ObjectGroup(const char* shaderFolder) :
	mShader(shaderFolder), mShadowMapShader("ShadowMap")
{
	glm::vec4 lightColor = glm::vec4(100.0f, 100.0f, 100.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(10.0f, 13.0f, 0.5f);

	mShader.UseProgram();
	glUniform4f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	mShadowMap.Initialize(lightPos);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
}

void ObjectGroup::ShadowMapUpdate(Camera& camera, unsigned int winWidth, unsigned int winHeight)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, mShadowMap.GetShadowMapWidth(), mShadowMap.GetShadowMapHeight());
	/*glBindFramebuffer(GL_FRAMEBUFFER, mShadowMap);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Update(mShadowMapShader, camera);
	}
	for (int i = 0; i < mFlatModels.size(); ++i)
	{
		mFlatModels[i].Update(mShadowMapShader, camera);
	}*/
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMap.GetFrameBuffer());
	glClear(GL_DEPTH_BUFFER_BIT);

	for (Model& model : mModels)
	{
		mShadowMap.UpdateShadows(model, camera);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, winWidth, winHeight);
}

void ObjectGroup::Update(Camera& camera)
{
	mShader.UseProgram();
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "lightProj"), 1, GL_FALSE, glm::value_ptr(mShadowMap.GetLightProj()));
	glUniform1f(UniformHandler::GetUniformLocation(mShader.GetID(), "farPlane"), mShadowMap.GetFarPlane());

	/*glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, mShadowMapTex);
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "shadowMap"), 2);*/

	glActiveTexture(GL_TEXTURE0 + 2);
	mShadowMap.BindTexture();
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "shadowCubeMap"), 2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	mShadowMap.Update(camera);

	for (Model& model : mModels)
	{
		model.Update(mShader, camera);
	}
	glDisable(GL_CULL_FACE);
	for (Model& model : mFlatModels)
	{
		model.Update(mShader, camera);
	}
}

void ObjectGroup::AddModel
(
	const char* file, const char* folder,
	unsigned int instances,
	std::vector<glm::mat4> instanceMatrix
)
{
	mModels.emplace_back(Model(file, folder, instances, instanceMatrix));
}

void ObjectGroup::AddFlatModel
(
	const char* file, const char* folder,
	unsigned int instances,
	std::vector<glm::mat4> instanceMatrix
)
{
	mFlatModels.emplace_back(Model(file, folder, instances, instanceMatrix));
}

ObjectGroup::~ObjectGroup()
{
	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Delete();
	}
	for (int i = 0; i < mFlatModels.size(); ++i)
	{
		mFlatModels[i].Delete();
	}
}
