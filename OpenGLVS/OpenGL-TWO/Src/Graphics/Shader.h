#pragma once
#include <string>

class Shader
{
public:
	Shader(const char* folder);
	~Shader();

	void UseProgram();

	unsigned int GetID();

private:
	std::string ParseFile(std::string fileName);
	void Check4Errors(unsigned int shader, const char* type);

	unsigned int mID;

};