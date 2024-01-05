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
#include "Camera.h"

GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,    0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

	 0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,	 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, 0.5f,    0.0f, 0.0f, 1.0f,   2.0f, 1.0f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.5f,    0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   1.0f, 0.0f, 0.0f
};

GLuint indices[] =
{
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,
};

GLfloat lightVertices[] =
{
	-1.0f, -0.5f, 1.0f,
	 0.0f, -0.5f, 1.0f,
	 0.0f,  0.5f, 1.0f,
	-1.0f,  0.5f, 1.0f,
};

GLuint lightIndices[] =
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

	vA.LinkVertexBuffer(vB, 0, 3, 11 * sizeof(float), (void*)0);
	vA.LinkVertexBuffer(vB, 1, 3, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	vA.LinkVertexBuffer(vB, 2, 2, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	vA.LinkVertexBuffer(vB, 3, 3, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	vA.Unbind();
	vB.Unbind();
	eB.Unbind();

	Shader lightShader("LightVertex.shader", "LightFragment.shader");

	VertexArray lightVA;
	lightVA.Bind();

	VertexBuffer lightVB(lightVertices, sizeof(lightVertices));
	ElementBuffer lightEB(lightIndices, sizeof(lightIndices));

	lightVA.LinkVertexBuffer(lightVB, 0, 3, 3 * sizeof(float), (void*)0);

	lightVA.Unbind();
	lightVB.Unbind();
	lightEB.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(-0.5f, 0.0f, 1.0f);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
	glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), objectPos);

	lightShader.UseProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.GetID(), "_model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.UseProgram();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.GetID(), "_model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.GetID(), "_lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.GetID(), "_lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture texture("Res/Textures/Image_Two.png", 0, GL_RGB);
	texture.TextureUnit(shaderProgram, "tex0", 0);
	Texture specTexture("Res/Textures/planksSpec.png", 1, GL_RED);
	specTexture.TextureUnit(shaderProgram, "tex1", 1);

	glEnable(GL_DEPTH_TEST);

	Camera camera(winSize.first, winSize.second, glm::vec3(0.0f, 0.0f, 2.0f));
	camera.SetMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "_mvp");
	camera.SetMatrix(45.0f, 0.1f, 100.0f, lightShader, "_mvp");
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		shaderProgram.UseProgram();
		glUniform3f(glGetUniformLocation(shaderProgram.GetID(), "_camPos"), camera.GetPos().x, camera.GetPos().y, camera.GetPos().z);
		camera.Update(shaderProgram, "_mvp");
		texture.Bind();
		specTexture.Bind();
		vA.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.UseProgram();
		camera.Update(lightShader, "_mvp");
		lightVA.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}