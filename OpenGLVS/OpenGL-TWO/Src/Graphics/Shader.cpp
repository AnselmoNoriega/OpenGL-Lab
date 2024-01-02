#include "Shader.h"
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* vertFile, const char* fragFile)
{
	std::string vertexCode = ParseFile(vertFile);
	std::string fragmentCode = ParseFile(fragFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	Check4Errors(vertexShader, "Vertex");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	Check4Errors(fragmentShader, "Fragment");

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);
	Check4Errors(_id, "Program");

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

unsigned int Shader::GetID()
{
	return _id;
}

void Shader::Check4Errors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

	if (hasCompiled == GL_FALSE && type != "Program")
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "SHADER COMPILATION ERROR: " << type << std::endl;
		return;
	}
	else if (hasCompiled == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "SHADER LINKING ERROR: " << type << std::endl;
		return;
	}
}

std::string Shader::ParseFile(const char* fileName)
{
	std::string path = std::string("Res/Shaders/") + fileName;
	std::ifstream in(path, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	return std::string();
}

