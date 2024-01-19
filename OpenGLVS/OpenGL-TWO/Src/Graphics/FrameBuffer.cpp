#include "FrameBuffer.h"

#include <glad/glad.h>
#include <iostream>

#include "UniformHandler.h"

FrameBuffer::FrameBuffer(const char* vertexShader, const char* fragmentShader, int winWidth, int winHeight, std::vector<Vertex>& vertices):
	mShader(vertexShader, fragmentShader), mVertexBuffer(vertices), mTexure(winWidth, winHeight)
{
	mShader.UseProgram();
	glUniform1i(UniformHandler::GetUniformLocation(mShader.GetID(), "screenTexture"), 0);

	mVertexArray.Bind();
	mVertexArray.LinkVertexBuffer(mVertexBuffer, 0, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	mVertexArray.LinkVertexBuffer(mVertexBuffer, 1, 2, sizeof(Vertex), (void*)(offsetof(Vertex, TextUV)));

	glGenFramebuffers(1, &mID);
	Bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexure.GetID(), 0);

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
	mTexure.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
