#pragma once

class Shader;

class Texture
{
public:
	Texture(const char* image);

	void TextureUnit(Shader& shader, const char* uniform, unsigned int unit);
	void Bind();
	void Unbind();
	void Delete();

private:
	unsigned int _id;
};

