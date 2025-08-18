//
// Created by jeanhua on 2025/8/18.
//

#ifndef MINECRAFT_TEXTURE_CUBE_H
#define MINECRAFT_TEXTURE_CUBE_H

#include "../../core.h"
#include <vector>

class TextureCube {
public:
    explicit TextureCube(const std::vector<std::string>& paths,unsigned int unit);
    ~TextureCube();

    void bind()const;

private:
    GLuint mTexture=0;
    uint32_t mUnit=0;
};

#endif