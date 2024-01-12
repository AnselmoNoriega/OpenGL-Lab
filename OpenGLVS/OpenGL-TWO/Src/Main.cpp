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
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);

	Model model("scene.gltf");

	Shader shaderProgram("VertexShader.shader", "FragmentShader.shader");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	shaderProgram.UseProgram();
	glUniform4f(UniformHandler::GetUniformLocation(shaderProgram.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(UniformHandler::GetUniformLocation(shaderProgram.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(UniformHandler::GetUniformLocation(shaderProgram.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera(winSize.first, winSize.second, glm::vec3(0.0f, 0.0f, 2.0f));
	camera.SetMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "_mvp");


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		model.Update(shaderProgram, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}