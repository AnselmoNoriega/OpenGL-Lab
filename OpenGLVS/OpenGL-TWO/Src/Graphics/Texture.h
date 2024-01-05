#pragma once

class Shader;

class Texture
{
public:
	Texture(const char* image, unsigned int unit);
	~Texture();

	void TextureUnit(Shader& shader, const char* uniform, unsigned int unit);
	void Bind();
	void Unbind();

private:
	unsigned int mID;
	unsigned int mUnit;
};

