//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H
#include "../../core.h"

class Texture {
public:
	Texture(const std::string& path, unsigned int unit);
	~Texture();

	void bind();
private:
	GLuint mTexture = 0;
	int mWidth = 0;
	int mHeight = 0;
	unsigned int mUnit = 0;
};

#endif