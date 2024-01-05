#include "Mesh.h"
#include "ElementBuffer.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
	mVertices = vertices;
	mIndices = indices;
	mTextures = textures;

	mVertexArray.Bind();

	VertexBuffer vB(vertices);
	ElementBuffer eB(indices);

	mVertexArray.LinkVertexBuffer(vB, 0, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
	mVertexArray.LinkVertexBuffer(vB, 1, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
	mVertexArray.LinkVertexBuffer(vB, 2, 3, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	mVertexArray.LinkVertexBuffer(vB, 3, 2, sizeof(Vertex), (void*)(offsetof(Vertex, TextUV)));

	mVertexArray.Unbind();
	vB.Unbind();
	eB.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
}
