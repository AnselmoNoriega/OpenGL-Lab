#pragma once
#include <unordered_map>

class UniformHandler
{
public:
	static unsigned int GetUniformLocation(unsigned int shaderID, const char* name);

private:
	static std::unordered_map<unsigned int, std::unordered_map<const char*, unsigned int>> mUniformLocationCache;
};