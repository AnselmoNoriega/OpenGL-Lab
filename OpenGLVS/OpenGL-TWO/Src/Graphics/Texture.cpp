#include "Texture.h"
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Shader.h"

Texture::Texture(const char* image, unsigned int unit)
{
	int widthImg, heightImg, colorChannelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgBytes = stbi_load(image, &widthImg, &heightImg, &colorChannelNum, 0);

	glGenTextures(1, &mID);
	glActiveTexture(GL_TEXTURE0 + unit);
	mUnit = unit;
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float tempColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, tempColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, imgBytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgBytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
}

void Texture::TextureUnit(Shader& shader, const char* uniform, unsigned int unit)
{
	GLuint tex0ID = glGetUniformLocation(shader.GetID(), uniform);
	shader.UseProgram();
	glUniform1i(tex0ID, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}