#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* vertFile, const char* fragFile);
	~Shader();

	void UseProgram();

	unsigned int GetID();

private:
	std::string ParseFile(const char* fileName);
	void Check4Errors(unsigned int shader, const char* type);

	unsigned int mID;

};