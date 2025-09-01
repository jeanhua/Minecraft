//
// Created by jeanhua on 2025/8/18.
//

#include "textureCube.h"
#include "stb/stb_image.h"
#include <filesystem>

TextureCube::~TextureCube() {
    if(mTexture!=0) {
        glDeleteTextures(1, &mTexture);
    }
}

TextureCube::TextureCube(const std::vector<std::string>& paths,unsigned int unit) {
    if (paths.size()!=6) {
        std::cerr<<"TEXT CUBE SHOULD BE 6 PATH"<<std::endl;
    }
    for (std::string_view p:paths) {
        if (!std::filesystem::exists(p)) {
            std::cerr << "File does not exist: " << p << std::endl;
        }
    }
    this->mUnit=unit;
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < paths.size(); i++) {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, STBI_default);
        if (data!=nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }else {
            std::cout << "Failed to load texture: " << paths[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
}

void TextureCube::bind() const {
    glActiveTexture(GL_TEXTURE0+mUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
}
