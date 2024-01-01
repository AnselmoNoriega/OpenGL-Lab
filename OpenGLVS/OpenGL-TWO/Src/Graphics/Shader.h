#pragma once


class Shader
{
public:
	Shader(const char* vertFile, const char* fragFile);

	void UseProgram();
	void Delete();

private:
	const char* ParseFile(const char* fileName);

	unsigned int _id;

};