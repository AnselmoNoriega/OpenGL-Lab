#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const char* vertFile, const char* fragFile)
{
}

void Shader::Start()
{
}

void Shader::Delete()
{
}

std::string Shader::ParseFile(const char* fileName)
{
	std::ifstream in(fileName, std::ios::binary);
	if (in)
	{
		std::string content;
		in.seekg(0, std::ios::end);
		content.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&content[0], content.size());
		in.close();
		return content;
	}

	return std::string();
}
