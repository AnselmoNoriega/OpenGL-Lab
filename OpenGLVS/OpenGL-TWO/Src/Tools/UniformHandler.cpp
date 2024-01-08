#include "UniformHandler.h"
#include <iostream>
#include <glad/glad.h>

std::unordered_map<unsigned int, std::unordered_map<const char*, unsigned int>> UniformHandler::mUniformLocationCache;

unsigned int UniformHandler::GetUniformLocation(unsigned int shaderID, const char* name)
{
	if (mUniformLocationCache[shaderID].find(name) != UniformHandler::mUniformLocationCache[shaderID].end())
	{
		return mUniformLocationCache[shaderID][name];
	}

	int location = glGetUniformLocation(shaderID, name);

	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist." << std::endl;
	}

	mUniformLocationCache[shaderID][name] = location;

	return location;
}
