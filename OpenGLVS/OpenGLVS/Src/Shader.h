#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
public:
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);

	void SetUniformMat3f(const std::string& name, glm::mat3& value);
	void SetUniformMat4f(const std::string& name, glm::mat4& value);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	int GetUniformLocation(const std::string& name);
	
	std::string mFilePath;
	unsigned int mRendererID;
	std::unordered_map<std::string, int> mUniformLocationCache;
};

