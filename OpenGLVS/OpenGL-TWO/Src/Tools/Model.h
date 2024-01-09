#pragma once
#include <json/json.h>
#include <glm/glm.hpp>
#include "VertexBuffer.h"
#include "Texture.h"

class Shader;
class Camera;
using json = nlohmann::json;

class Model
{
public:
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:
	const char* mFile;
	std::vector<unsigned char> mData;
	nlohmann::json mJson;

	std::vector<std::string>mLoadedTexName;
	std::vector<Texture> mLoadedTex;

	std::vector<unsigned char> GetData(); 
	std::vector<float> GetFloats(json accessor);
	std::vector<unsigned int> GetIndices(json accessor);
	std::vector<Texture> GetTextures();

	std::vector<Vertex> AsssembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec3> texUVs
	);

	std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);
	
	std::string ParseFile(const char* fileName);
};