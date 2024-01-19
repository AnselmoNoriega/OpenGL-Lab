#pragma once

#include <glad/glad.h>
class Shader;

class Texture
{
public:
	Texture(const char* image,const char* texType, unsigned int unit);
	Texture(int winWidth, int winHeight);

	void TextureUnit(Shader& shader, const char* uniform, unsigned int unit);
	void Bind();
	void Unbind();
	void Delete();

	const char* GetTextureType() { return mTexType; }

	unsigned int GetID() { return mID; }

private:
	unsigned int mID;
	unsigned int mUnit;
	const char* mTexType;
};

