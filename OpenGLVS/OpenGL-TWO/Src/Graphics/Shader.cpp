#include "Shader.h"
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

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
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

