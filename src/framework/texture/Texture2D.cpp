//
// Created by jeanhua on 2025/8/16.
//

#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <filesystem>

Texture2D::Texture2D(const std::string& path, unsigned int unit) :mUnit(unit)
{
	if (!std::filesystem::exists(path)) {
		std::cerr << "File does not exist: " << path << std::endl;
	}
	int channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);
	if (imageData == nullptr) {
		std::cout << "read image error" << std::endl;
	}
	glGenTextures(1, &mTexture);
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //v

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	if (mTexture != 0) {
		glDeleteTextures(1, &mTexture);
	}
}

void Texture2D::bind()const
{
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
