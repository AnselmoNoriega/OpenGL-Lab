#include "Shader.h"
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* folder)
{
	std::string vertexCode = ParseFile(folder + std::string("/Vertex.shader"));
	std::string fragmentCode = ParseFile(folder + std::string("/Fragment.shader"));

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

	mID = glCreateProgram();
	glAttachShader(mID, vertexShader);
	glAttachShader(mID, fragmentShader);
	glLinkProgram(mID);
	Check4Errors(mID, "Program");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(mID);
}

void Shader::UseProgram()
{
	glUseProgram(mID);
}

unsigned int Shader::GetID()
{
	return mID;
}

void Shader::Check4Errors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

	if (hasCompiled == GL_FALSE && type != "Program")
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "SHADER COMPILATION ERROR: " << type << ": " << infoLog << std::endl;
		return;
	}
	else if (hasCompiled == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "SHADER LINKING ERROR: " << type << ": " << infoLog << std::endl;
		return;
	}
}

std::string Shader::ParseFile(std::string fileName)
{
	std::string path = "Res/Shaders/" + fileName;
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

