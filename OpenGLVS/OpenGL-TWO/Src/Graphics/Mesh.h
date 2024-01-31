#pragma once
#include "VertexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <glm/gtc/type_ptr.hpp>

class Camera;

class Mesh
{
public:
	Mesh
	(
		std::vector<Vertex>& vertices, 
		std::vector<unsigned int>& indices, 
		std::vector<Texture>& textures,
		unsigned int instances = 1,
		std::vector<glm::mat4> instanceMatrix = {}
	);

	void Delete();

	void Draw
	(
		Shader& shader, 
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		bool isLightShader = false
	);

private:
	std::vector <Vertex> mVertices;
	std::vector <unsigned int> mIndices;
	std::vector <Texture> mTextures;

	VertexArray mVertexArray;

	unsigned int mCamID;

	unsigned int mInstances;
};

