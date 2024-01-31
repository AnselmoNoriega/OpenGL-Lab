#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Color = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 TextUV = glm::vec2(0.0f, 0.0f);
};

class VertexBuffer
{
public:
	VertexBuffer(std::vector<Vertex>& vertices);
	VertexBuffer(std::vector<glm::mat4>& mat4s);
	~VertexBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int mID;
};

