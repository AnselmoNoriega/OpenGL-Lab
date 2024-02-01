#include "FrameBuffer.h"

#include <glad/glad.h>
#include <iostream>

#include "UniformHandler.h"

Vertex rectangleVertices[] =
{
	Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

	Vertex(glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
};
std::vector<Vertex> frameRec(rectangleVertices, rectangleVertices + sizeof(rectangleVertices) / sizeof(Vertex));

FrameBuffer::FrameBuffer(const char* shaderFolder, int winWidth, int winHeight, int samplesNum):
	mShader(shaderFolder), mVertexBuffer(frameRec)
{
	mShader.UseProgram();
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "screenTexture"), 0);

	mVertexArray.Bind();
	mVertexArray.LinkVertexBuffer(mVertexBuffer, 0, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	mVertexArray.LinkVertexBuffer(mVertexBuffer, 1, 2, sizeof(Vertex), (void*)(offsetof(Vertex, TextUV)));

	glGenFramebuffers(1, &mSampleID);
	glBindFramebuffer(GL_FRAMEBUFFER, mSampleID);

	unsigned int frameTexture;
	glGenTextures(1, &frameTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, frameTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplesNum, GL_RGB, winWidth, winHeight, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, frameTexture, 0);

	unsigned int RB;
	glGenRenderbuffers(1, &RB);
	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplesNum, GL_DEPTH24_STENCIL8, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);

	auto FbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer ERROR = " << FbStatus << std::endl;
	}

	glGenFramebuffers(1, &mPostProcessingID);
	glBindFramebuffer(GL_FRAMEBUFFER, mPostProcessingID);

	glGenTextures(1, &mPostProcessingTextureID);
	glBindTexture(GL_TEXTURE_2D, mPostProcessingTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPostProcessingTextureID, 0);

	auto FbPosStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FbPosStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer ERROR = " << FbPosStatus << std::endl;
	}
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mSampleID);
}

void FrameBuffer::Update(int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mSampleID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mPostProcessingID);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mShader.UseProgram();
	mVertexArray.Bind();
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, mPostProcessingTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
