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

	Camera camera(winSize.first, winSize.second, glm::vec3(0.0f, 0.0f, 2.0f));
	camera.SetMatrix(45.0f, 0.1f, 100.0f);

	ObjectGroup objects("Normal");
	objects.AddModel("Bird/scene.gltf", "Bird/");
	objects.AddFlatModel("Grass/scene.gltf", "Grass/");
	objects.AddModel("Ground2/scene.gltf", "Ground2/");

	SkyBox skyBox("Skybox", winSize.first, winSize.second);

	FrameBuffer frameBuffer("PostProcessing", winSize.first, winSize.second, 32);
	{
		float gammaVal = frameBuffer.GetGammaValue();
		glClearColor(std::pow(0.9f, gammaVal), std::pow(0.7f, gammaVal), std::pow(1.0f, gammaVal), 1.0f);
	}

	while (!glfwWindowShouldClose(window))
	{
		objects.ShadowMapUpdate(camera, winSize.first, winSize.second);

		frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.Inputs(window);

		objects.Update(camera);

		skyBox.Update(camera);

		frameBuffer.Update(winSize.first, winSize.second);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}