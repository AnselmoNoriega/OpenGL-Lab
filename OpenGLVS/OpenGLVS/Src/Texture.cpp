#include "Texture.h"
#include "Vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: mRendererID(0), mFilePath(path), mLocalBuffer(nullptr), mWidth(0), mHeight(0), mBPP(0)
{
	stbi_set_flip_vertically_on_load(true);
	mLocalBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBPP, 4);

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (mLocalBuffer)
	{
		stbi_image_free(mLocalBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &mRendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
