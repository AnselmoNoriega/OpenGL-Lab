#include "SpotLight.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "UniformHandler.h"
#include "Model.h"

SpotLight::SpotLight() :
	mShader("ShadowCubeMap", true)
{

}

void SpotLight::Initialize
(
	unsigned int shadowMapWidth,
	unsigned int shadowMapHeight,
	float farplane,
	glm::vec3 lightPos
)
{
	glGenFramebuffers(1, &mFrameBufferID);

	glGenTextures(1, &mTextureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farplane);
	glm::mat4 shadowTransforms[] =
	{
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
		shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f)),
	};

	mShader.UseProgram();
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "_shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
	glUniform3f(UniformHandler::GetUniformLocation(mShader.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(UniformHandler::GetUniformLocation(mShader.GetID(), "_farPlane"), farplane);
}

void SpotLight::BindTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
}
