#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "ObjectTypes/ObjectGroup.h"
#include "FrameBuffer.h"
#include "SkyBox.h"

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

	/*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);*/

	Camera camera(winSize.first, winSize.second, glm::vec3(0.0f, 0.0f, 2.0f));
	camera.SetMatrix(45.0f, 0.1f, 100.0f);

	Shader skyboxShader("VertexSkybox.shader", "FragmentSkybox.shader");
	skyboxShader.UseProgram();
	glUniform1i(glGetUniformLocation(skyboxShader.GetID(), "skybox"), 0);

	ObjectGroup objects("VertexShader.shader", "FragmentShader.shader");
	objects.AddModel("Bird/scene.gltf", "Bird/");
	objects.AddModel("Grass/scene.gltf", "Grass/");
	objects.AddModel("Ground2/scene.gltf", "Ground2/");

	SkyBox skyBox("VertexSkybox.shader", "FragmentSkybox.shader", winSize.first, winSize.second);

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

	FrameBuffer frameBuffer("VertexBasic.shader", "FragmentBasic.shader", winSize.first, winSize.second, frameRec);

	while (!glfwWindowShouldClose(window))
	{
		frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.Inputs(window);

		objects.Update(camera);

		skyBox.Update(camera);

		frameBuffer.Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}