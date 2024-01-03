#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"

GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,    0.5f, 0.5f, 0.5f,   0.0f, 1.0f
};

GLuint indices[] =
{
	0, 1, 2,
	2, 3, 0,
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::pair<const unsigned int, const unsigned int> winSize(800, 800);
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

	Shader shaderProgram("VertexShader.shader", "FragmentShader.shader");

	VertexArray vA;
	vA.Bind();

	VertexBuffer vB(vertices, sizeof(vertices));
	ElementBuffer eB(indices, sizeof(indices));

	vA.LinkVertexBuffer(vB, 0, 3, 8 * sizeof(float), (void*)0);
	vA.LinkVertexBuffer(vB, 1, 3, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vA.LinkVertexBuffer(vB, 2, 2, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vA.Unbind();
	vB.Unbind();
	eB.Unbind();

	Texture texture("Res/Textures/Image_Two.png");
	texture.TextureUnit(shaderProgram, "tex0", 0);

	shaderProgram.UseProgram();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(winSize.first/ winSize.second), 0.1f, 100.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = proj * view * model;

	int MVP_ID = glGetUniformLocation(shaderProgram.GetID(), "_mvp");
	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, glm::value_ptr(MVP));

	texture.Bind();
	vA.Bind();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vA.Delete();
	vB.Delete();
	eB.Delete();
	texture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}