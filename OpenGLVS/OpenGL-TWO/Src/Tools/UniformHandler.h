#pragma once
#include <unordered_map>
#include <string>

class UniformHandler
{
public:
	static unsigned int GetUniformLocation(unsigned int shaderID, std::string name);

private:
	static std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>> mUniformLocationCache;
};