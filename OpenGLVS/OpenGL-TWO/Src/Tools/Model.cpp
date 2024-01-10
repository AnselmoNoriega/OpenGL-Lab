#include "Model.h"
#include <iostream>
#include <fstream>

Model::Model(const char* file)
{
	std::string text = ParseFile(file);
	mJson = json::parse(text);

	mFile = file;
	mData = GetData();
	TraverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	for (unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Mesh::Draw(shader, camera, mMatricesMeshes[i]);
	}
}

void Model::LoadMesh(unsigned int meshInd)
{
	unsigned int posAccInd = mJson["meshes"][meshInd]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = mJson["meshes"][meshInd]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = mJson["meshes"][meshInd]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = mJson["meshes"][meshInd]["primitives"][0]["indices"];

	std::vector<float> posVec = GetFloats(mJson["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = GroupFloatsVec3(posVec);
	std::vector<float> normalsVec = GetFloats(mJson["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = GroupFloatsVec3(normalsVec);
	std::vector<float> texVec = GetFloats(mJson["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = GroupFloatsVec2(texVec);

	std::vector<Vertex> vertices = AsssembleVertices(positions, normals, texUVs);
	std::vector<unsigned int> indices = GetIndices(mJson["accessors"][indAccInd]);
	std::vector<Texture> textures = GetTextures();

	mMeshes.push_back(Mesh(vertices, indices, textures));
}

void Model::TraverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	json node = mJson["nodes"][nextNode];

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end())
	{
		float transValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); ++i)
		{
			transValues[i] = (node["translation"][i]);
		}
		translation = glm::make_vec3(transValues);
	}
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end())
	{
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); ++i)
		{
			scaleValues[i] = (node["matrix"][i]);
		}
		scale = glm::make_vec3(scaleValues);
	}
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end())
	{
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); ++i)
		{
			matValues[i] = (node["matrix"][i]);
		}
		matNode = glm::make_mat4(matValues);
	}

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rot = glm::mat4_cast(rotation);
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	if (node.find("mesh") != node.end())
	{
		mTranslationsMeshes.push_back(translation);
		mRotationsMeshes.push_back(rotation);
		mScalesMeshes.push_back(scale);
		mMatricesMeshes.push_back(matNextNode);

		LoadMesh(node["mesh"]);
	}

	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); ++i)
		{
			TraverseNode(node["children"][i], matNextNode);
		}
	}
}

std::vector<unsigned char> Model::GetData()
{
	std::string uri = mJson["buffers"][0]["uri"];

	std::string fileStr = std::string(mFile);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
	std::string bytesText = ParseFile((fileDirectory + uri).c_str());

	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::GetFloats(json accessor)
{
	std::vector<float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = mJson["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR")
	{
		numPerVert = 1;
	}
	else if (type == "VEC2")
	{
		numPerVert = 2;
	}
	else if (type == "VEC3")
	{
		numPerVert = 3;
	}
	else if (type == "VEC4")
	{
		numPerVert = 4;
	}
	else
	{
		std::invalid_argument("Type of Model is Invalid");
	}

	unsigned int beginningOfData = byteOffset + accByteOffset;
	unsigned int lengthOfData = count * 4 * numPerVert;
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<unsigned int> Model::GetIndices(json accessor)
{
	std::vector<unsigned int> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	unsigned int commponentType = accessor["componentType"];

	json bufferView = mJson["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int begginingOfData = byteOffset + accByteOffset;
	switch (commponentType)
	{
	case 5125:
	{
		for (unsigned int i = begginingOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { mData[i++], mData[i++], mData[i++], mData[i++] };
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));
			indices.push_back(value);
		}
	}
	break;
	case 5123:
	{
		for (unsigned int i = begginingOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { mData[i++], mData[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back(value);
		}
	}
	break;
	case 5122:
	{
		for (unsigned int i = begginingOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { mData[i++], mData[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back(value);
		}
	}
	break;
	}

	return indices;
}

std::vector<Texture> Model::GetTextures()
{
	std::vector<Texture> textures;

	std::string fileStr = std::string(mFile);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	for (unsigned int i = 0; i < mJson["images"].size(); ++i)
	{
		std::string texPath = mJson["images"][i]["uri"];

		bool skip = false;
		for (unsigned int j = 0; j < mLoadedTexName.size(); ++j)
		{
			if (mLoadedTexName[j] == texPath)
			{
				textures.push_back(mLoadedTex[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuce = Texture((fileDirectory + texPath).c_str(), "diffuce", mLoadedTex.size());
				textures.push_back(diffuce);
				mLoadedTex.push_back(diffuce);
				mLoadedTexName.push_back(texPath);
			}
			else if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", mLoadedTex.size());
				textures.push_back(specular);
				mLoadedTex.push_back(specular);
				mLoadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<Vertex> Model::AsssembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); ++i)
	{
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				normals[i],
				texUVs[i]
			}
		);
	}
	return vertices;
}

std::vector<glm::vec2> Model::GroupFloatsVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec3> Model::GroupFloatsVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::vector<glm::vec4> Model::GroupFloatsVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> vectors;
	for (int i = 0; i < floatVec.size(); i)
	{
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return vectors;
}

std::string Model::ParseFile(const char* fileName)
{
	std::string path = std::string("Res/Models/") + fileName;
	std::ifstream in(path, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	return std::string();
}
