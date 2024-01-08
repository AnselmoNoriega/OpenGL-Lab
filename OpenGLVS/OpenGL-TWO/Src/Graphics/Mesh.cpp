#include "Mesh.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "UniformHandler.h"

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
	shader.UseProgram();
	mVertexArray.Bind();

	unsigned int diffNum = 0;
	unsigned int specNum = 0;

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		std::string num;
		std::string texType = mTextures[i].GetTextureType();

		if (texType == "diffuce")
		{
			num = std::to_string(diffNum++);
		}
		else if (texType == "specular")
		{
			num = std::to_string(specNum++);
		}

		mTextures[i].TextureUnit(shader, (texType + num).c_str(), i);
		mTextures[i].Bind();
	}

	glUniform3f(UniformHandler::GetUniformLocation(shader.GetID(), "_camPos"), camera.GetPos().x, camera.GetPos().y, camera.GetPos().z);
	camera.Update(shader, "_mvp");

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}
