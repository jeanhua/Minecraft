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

    void render();

    // control
    bool showFog = true;
    bool showSunshine = true;
    ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.8;
    float specularStrength = 0.9;
    int shininess = 32;
    bool showSkybox = true;
    std::vector<std::string>blocks;
    int currentBlock = 0;

    bool drawLine = false;

private:
    GLuint mVAO=0,mVBO=0,mEBO=0;
    int indicesCount=0;
    Shader* mShader=nullptr;

    Texture2D* worldTexture;

    void genBuffers();
    void buildUIFrame();
};

#endif //MINECRAFT_USER_INTERFACE_H