#pragma once
#include "VertexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"

class Camera;

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	void Draw(Shader& shader, Camera& camera);

private:
	std::vector <Vertex> mVertices;
	std::vector <unsigned int> mIndices;
	std::vector <Texture> mTextures;

	VertexArray mVertexArray;

	unsigned int mCamID;
};

