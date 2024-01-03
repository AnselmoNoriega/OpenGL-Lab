#include "Texture.h"
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Shader.h"

Texture::Texture(const char* image)
{
	int widthImg, heightImg, colorChannelNum;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgBytes = stbi_load(image, &widthImg, &heightImg, &colorChannelNum, 0);

	glGenTextures(1, &_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);

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

void Texture::TextureUnit(Shader& shader, const char* uniform, unsigned int unit)
{
	GLuint tex0ID = glGetUniformLocation(shader.GetID(), uniform);
	shader.UseProgram();
	glUniform1i(tex0ID, unit);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &_id);
}
