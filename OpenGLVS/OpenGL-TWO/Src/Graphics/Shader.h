#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* vertFile, const char* fragFile);

	void UseProgram();
	void Delete();

private:
	std::string ParseFile(const char* fileName);

	unsigned int _id;

};