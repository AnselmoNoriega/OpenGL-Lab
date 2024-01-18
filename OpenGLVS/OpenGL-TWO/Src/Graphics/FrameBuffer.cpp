#include "FrameBuffer.h"

#include <glad/glad.h>
#include <iostream>

#include "UniformHandler.h"
#include "VertexBuffer.h"

FrameBuffer::FrameBuffer(const char* vertexShader, const char* fragmentShader, int winWidth, int winHeight):
	mShader(vertexShader, fragmentShader)
{
	mShader.UseProgram();
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "screenTexture"), 0);

	Vertex rectangleVertices[] =
	{
		Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

		Vertex(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f))
	};

	std::vector<Vertex> frameRec(rectangleVertices, rectangleVertices + sizeof(rectangleVertices) / sizeof(Vertex));
	VertexBuffer vertexBuffer(frameRec);

	mVertexArray.Bind();
	mVertexArray.LinkVertexBuffer(vertexBuffer, 0, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	mVertexArray.LinkVertexBuffer(vertexBuffer, 1, 2, sizeof(Vertex), (void*)(offsetof(Vertex, TextUV)));

	glGenFramebuffers(1, &mID);
	Bind();

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

	unsigned int RB;
	glGenRenderbuffers(1, &RB);
	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);

	auto FbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer ERROR = " << FbStatus << std::endl;
	}
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mID);
}

void FrameBuffer::Update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mShader.UseProgram();
	mVertexArray.Bind();
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
