#define GLFW_INCLUDE_NONE
#include "SkyBox.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Camera.h"
#include "UniformHandler.h"

#include <stb/stb_image.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

SkyBox::SkyBox(const char* vertexShader, const char* fragmentShader, int screenWidth, int screenHeight):
	mShader(vertexShader, fragmentShader), mScreenWidth(screenWidth), mScreenHeight(screenHeight)
{
	Vertex skyboxVertices[] =
	{
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
		Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
		Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f))
	};

	unsigned int skyboxIndices[] =
	{
		1, 2, 6,
		6, 5, 1,

		0, 4, 7,
		7, 3, 0,

		4, 5, 6,
		6, 7, 4,

		0, 3, 2,
		2, 1, 0,

		0, 1, 5,
		5, 4, 0,

		3, 7, 6,
		6, 2, 3
	};

	std::vector<Vertex> skyBoxVert(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(Vertex));
	std::vector<unsigned int> skyBoxInd(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(unsigned int));

	mVertexArray.Bind();
	VertexBuffer vb(skyBoxVert);
	ElementBuffer eb(skyBoxInd);
	mVertexArray.LinkVertexBuffer(vb, 0, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));

	vb.Unbind();
	mVertexArray.Unbind();
	eb.Unbind();

	std::string skyboxImgpath[6] =
	{
		"Res/Textures/CubeMapSky/right.jpg",
		"Res/Textures/CubeMapSky/left.jpg",
		"Res/Textures/CubeMapSky/top.jpg",
		"Res/Textures/CubeMapSky/bottom.jpg",
		"Res/Textures/CubeMapSky/front.jpg",
		"Res/Textures/CubeMapSky/back.jpg",
	};

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(skyboxImgpath[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed Loading CubeMap. Number [" << i << "]" << std::endl;
			stbi_image_free(data);
		}
	}
}

void SkyBox::Update(Camera& camera)
{
	glDepthFunc(GL_LEQUAL);
	mShader.UseProgram();
	glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.GetPos(), camera.GetPos() + camera.mOrientation, camera.up)));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mScreenWidth / mScreenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(mShader.GetID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	mVertexArray.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	mVertexArray.Unbind();
	
	glDepthFunc(GL_LESS);
}

SkyBox::~SkyBox()
{
	mVertexArray.Delete();
	glDeleteTextures(1, &mTextureID);
}