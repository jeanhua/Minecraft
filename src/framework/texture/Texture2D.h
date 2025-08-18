//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_TEXTURE2D_H
#define MINECRAFT_TEXTURE2D_H
#include "../../core.h"

class Texture2D {
public:
	Texture2D(const std::string& path, unsigned int unit);
	~Texture2D();

	void bind();
private:
	GLuint mTexture = 0;
	int mWidth = 0;
	int mHeight = 0;
	unsigned int mUnit = 0;
};

#endif