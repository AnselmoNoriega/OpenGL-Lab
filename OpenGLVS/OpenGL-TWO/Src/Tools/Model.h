#pragma once
#include <json/json.h>

#include "VertexBuffer.h"
#include "Texture.h"
#include "Mesh.h"

class Shader;
class Camera;
class Vertex;

using json = nlohmann::json;

class Model
{
public:
	Model(const char* file, const char* folder = "");
	void Delete();

	void Update(Shader& shader, Camera& camera);

private:
	const char* mFile;
	const char* mFolder;
	std::vector<unsigned char> mData;
	nlohmann::json mJson; 

	std::vector<Mesh> mMeshes;
	std::vector<glm::vec3> mTranslationsMeshes;
	std::vector<glm::quat> mRotationsMeshes;
	std::vector<glm::vec3> mScalesMeshes;
	std::vector<glm::mat4> mMatricesMeshes;

	std::vector<std::string>mLoadedTexName;
	std::vector<Texture> mLoadedTex;

	void LoadMesh(unsigned int meshInd);

	void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> GetData(); 
	std::vector<float> GetFloats(json accessor);
	std::vector<unsigned int> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	std::vector<Vertex> AssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);
	
	std::string ParseFile(const char* fileName);
};