//
// Created by jeanhua on 2025/8/19.
//

#ifndef MINECRAFT_USER_INTERFACE_H
#define MINECRAFT_USER_INTERFACE_H

#include "../../framework/framework.h"
#include "../../framework/shader/shader.h"
#include "../../framework/texture/texture2D.h"

class UserInterface {
public:
    explicit UserInterface(Texture2D* worldTexture);
    ~UserInterface();

    void render() const;

private:
    GLuint mVAO=0,mVBO=0,mEBO=0;
    int indicesCount=0;
    Shader* mShader=nullptr;

    Texture2D* worldTexture;

    void genBuffers();
};

#endif //MINECRAFT_USER_INTERFACE_H