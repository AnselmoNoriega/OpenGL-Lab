#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "UniformHandler.h"
#include "Model.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::pair<const unsigned int, const unsigned int> winSize(1000, 1000);
	GLFWwindow* window = glfwCreateWindow(winSize.first, winSize.second, "Task Two", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Window Failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, winSize.first, winSize.second);
	glClearColor(0.9f, 0.7f, 1.0f, 1.0f);

	std::vector<Model> models;
	models.emplace_back(Model("Bird/scene.gltf", "Bird/"));
	models.emplace_back(Model("Ground2/scene.gltf", "Ground2/"));
	models.emplace_back(Model("Grass/scene.gltf", "Grass/"));

	std::vector<Model> outlineModels;
	outlineModels.emplace_back(Model("BirdOutline/scene.gltf", "BirdOutline/"));

	Shader shaderProgram("VertexShader.shader", "FragmentShader.shader");
	Shader outliningProgram("VertexOutline.shader", "FragmentOutline.shader");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	shaderProgram.UseProgram();
	glUniform4f(UniformHandler::GetUniformLocation(shaderProgram.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(UniformHandler::GetUniformLocation(shaderProgram.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Camera camera(winSize.first, winSize.second, glm::vec3(0.0f, 0.0f, 2.0f));
	camera.SetMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "_mvp");

	unsigned int FB;
	glGenFramebuffers(1, &FB);
	glBindFramebuffer(GL_FRAMEBUFFER, FB);

	unsigned int FbTexture;
	glGenTextures(1, &FbTexture);
	glBindTexture(GL_TEXTURE_2D, FbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winSize.first, winSize.second, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FbTexture, 0);

	unsigned int RB;
	glGenRenderbuffers(1, &RB);
	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, winSize.first, winSize.second);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);

	auto FbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FrameBuffer ERROR = " << FbStatus << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.Inputs(window);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		for (int i = 0; i < models.size(); ++i)
		{
			models[i].Update(shaderProgram, camera);
		}

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outliningProgram.UseProgram();

		for (int i = 0; i < outlineModels.size(); ++i)
		{
			outlineModels[i].Update(outliningProgram, camera);
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int i = 0; i < models.size(); ++i)
	{
		models[i].Delete();
	}

	for (int i = 0; i < outlineModels.size(); ++i)
	{
		outlineModels[i].Delete();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}