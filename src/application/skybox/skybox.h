//
// Created by jeanhua on 2025/8/18.
//

#ifndef MINECRAFT_SKYBOX_H
#define MINECRAFT_SKYBOX_H

#include "../../core.h"
#include "../../framework/shader/shader.h"

class Skybox {
public:
    explicit Skybox(Shader* shader);
    ~Skybox();

    void render() const;

private:
    GLuint mVAO=0,mVBO=0;
    Shader* mShader;
};

#endif //MINECRAFT_SKYBOX_H