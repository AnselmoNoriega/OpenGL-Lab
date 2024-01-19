#include "Texture.h"
#include <iostream>
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Shader.h"
#include "UniformHandler.h"

Texture::Texture(const char* image, const char* texType, unsigned int unit)
{
	mTexType = texType;

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
	if (colorChannelNum == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBytes);
	}
	else if (colorChannelNum == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, imgBytes);
	}
	else if (colorChannelNum == 1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, imgBytes);
	}
	else
	{
		std::cout<<"Texture Type: Recognition Error\n";
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgBytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int winWidth, int winHeight)
{
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::TextureUnit(Shader& shader, const char* uniform, unsigned int unit)
{
	shader.UseProgram();
	glUniform1i(UniformHandler::GetUniformLocation(shader.GetID(), uniform), unit);
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


void Texture::Delete()
{
	glDeleteTextures(1, &mID);
}