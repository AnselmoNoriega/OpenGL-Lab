#pragma once

#include <glad/glad.h>
class Shader;

class Texture
{
public:
	Texture(const char* image,const char* texType, unsigned int unit);
	~Texture();

	void TextureUnit(Shader& shader, const char* uniform, unsigned int unit);
	void Bind();
	void Unbind();

	const char* GetTextureType() { return mTexType; }

private:
	unsigned int mID;
	unsigned int mUnit;
	const char* mTexType;
};

