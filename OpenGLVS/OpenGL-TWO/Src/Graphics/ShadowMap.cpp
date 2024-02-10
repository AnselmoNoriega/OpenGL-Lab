#include "ShadowMap.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/vector_angle.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "UniformHandler.h"
#include "Camera.h"

ShadowMap::ShadowMap():
	mShader("ShadowMap"), mLightShader("Light"), mShadowMapWidth(2048), mShadowMapHeight(2048), farPlane(100.0f)
{
}

void ShadowMap::Initialize(glm::vec3 lightPos)
{
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
	
	glm::mat4 orthogonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, farPlane);
	glm::mat4 perspectivrProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lightProj = perspectivrProj * lightView;
	
	mShader.UseProgram();
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "lightProj"), 1, GL_FALSE, glm::value_ptr(lightProj));
	
	mLightObj.ChangePos(lightPos);
	mSpotLight.Initialize(mShadowMapWidth, mShadowMapHeight, farPlane, lightPos);
}

void ShadowMap::Update(Camera& camera)
{
	mLightObj.Update(mLightShader, camera, true);
}

void ShadowMap::UpdateShadows(Model& model, Camera& camera)
{
	model.Update(mSpotLight.GetShader(), camera);
}

void ShadowMap::BindTexture()
{
	mSpotLight.BindTexture();
}

ShadowMap::~ShadowMap()
{
	mLightObj.Delete();
}
