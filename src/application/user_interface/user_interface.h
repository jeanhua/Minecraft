//
// Created by jeanhua on 2025/8/19.
//

#ifndef MINECRAFT_USER_INTERFACE_H
#define MINECRAFT_USER_INTERFACE_H

#include "../../core.h"
#include "../../framework/shader/shader.h"

class UserInterface {
public:
    UserInterface();
    ~UserInterface();

    void render() const;

private:
    GLuint mVAO=0,mVBO=0;
    Shader* mShader=nullptr;
};

#endif //MINECRAFT_USER_INTERFACE_H