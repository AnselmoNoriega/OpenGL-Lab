#include "ObjectGroup.h"
#include "ElementBuffer.h"
#include "UniformHandler.h"

ObjectGroup::ObjectGroup(const char* shaderFolder) :
	mShader(shaderFolder), mShadowMapShader("ShadowMap"), mLightShader("Light"), mLightObj(LightInit())
{
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(19.0f, 6.0f, 0.5f);
	mLightModel = glm::translate(glm::mat4(1.0f), lightPos);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	mShader.UseProgram();
	glUniform4f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glGenFramebuffers(1, &mShadowMap);

	glGenTextures(1, &mShadowMapTex);
	glBindTexture(GL_TEXTURE_2D, mShadowMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowMapWidth, mShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMap);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float farPlane = 100.0f;
	glm::mat4 orthogonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, farPlane);
	glm::mat4 perspectivrProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lightProj = perspectivrProj * lightView;

	mShadowMapShader.UseProgram();
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShadowMapShader.GetID(), "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));

}

void ObjectGroup::ShadowMapUpdate(Camera& camera, unsigned int winWidth, unsigned int winHeight)
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMap);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Update(mShadowMapShader, camera);
	}
	for (int i = 0; i < mFlatModels.size(); ++i)
	{
		mFlatModels[i].Update(mShadowMapShader, camera);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, winWidth, winHeight);
}

void ObjectGroup::Update(Camera& camera)
{
	mShader.UseProgram();
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, mShadowMapTex);
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "shadowMap"), 2);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	mLightObj.Draw(mLightShader, camera, true, mLightModel);
	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Update(mShader, camera);
	}
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < mFlatModels.size(); ++i)
	{
		mFlatModels[i].Update(mShader, camera);
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

Mesh ObjectGroup::LightInit()
{
	Vertex verts[] =
	{
		Vertex(glm::vec3( 1.0f, -1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, 0.0f)),

		Vertex(glm::vec3( 1.0f, -1.0f, 1.0f)),
		Vertex(glm::vec3( 1.0f,  1.0f, 1.0f)),

		Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 1.0f))
	};

	unsigned int indcs[] =
	{
		0, 1, 2,
		2, 3, 0,

		4, 0, 3,
		3, 5, 4,

		6, 4, 5,
		5, 7, 6,

		1, 6, 7,
		7, 2, 1,

		3, 2, 7,
		7, 5, 3,

		4, 6, 1, 
		1, 0, 4
	};

	std::vector<Vertex> vertices(verts, verts + sizeof(verts) / sizeof(Vertex));
	std::vector<unsigned int> indices(indcs, indcs + sizeof(indcs) / sizeof(unsigned int));

	return Mesh(vertices, indices);
}

ObjectGroup::~ObjectGroup()
{
	mLightObj.Delete();
	for (int i = 0; i < mModels.size(); ++i)
	{
		mModels[i].Delete();
	}
}
