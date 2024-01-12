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

void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale,
	bool isLightShader
)
{
	shader.UseProgram();
	mVertexArray.Bind();

	if (!isLightShader)
	{
		unsigned int diffNum = 0;
		unsigned int specNum = 0;

		for (size_t i = 0; i < mTextures.size(); ++i)
		{
			std::string num;
			std::string texType = mTextures[i].GetTextureType();

			if (texType == "diffuse")
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
	}

	camera.Update(shader, "_mvp");

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	glUniformMatrix4fv(UniformHandler::GetUniformLocation(shader.GetID(), "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(shader.GetID(), "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(shader.GetID(), "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(UniformHandler::GetUniformLocation(shader.GetID(), "_model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
	for (int i = 0; i < mTextures.size(); i++)
	{
		mTextures[i].Delete();
	}

	mVertexArray.Delete();
}
