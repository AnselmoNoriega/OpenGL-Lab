#pragma once


class Shader
{
public:
	Shader(const char* vertFile, const char* fragFile);

	void Start();
	void Delete();

private:
	std::string ParseFile(const char* fileName);

	unsigned int _id;

};