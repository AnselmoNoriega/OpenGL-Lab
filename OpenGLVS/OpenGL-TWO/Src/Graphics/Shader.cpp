#include "Shader.h"
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* vertFile, const char* fragFile)
{
	auto vertShaderText = ParseFile(vertFile);
	auto fragShaderText = ParseFile(fragFile);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderText, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderText, NULL);
	glCompileShader(fragmentShader);

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::UseProgram()
{
	glUseProgram(_id);
}

void Shader::Delete()
{
	glDeleteProgram(_id);
}

const char* Shader::ParseFile(const char* fileName)
{
	std::string path = std::string("Res/Shaders/") + fileName;
	std::ifstream in(path, std::ios::binary);
	if (in)
	{
		std::string content;
		in.seekg(0, std::ios::end);
		content.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return content.c_str();
	}

	return std::string().c_str();
}
