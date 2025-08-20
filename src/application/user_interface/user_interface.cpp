//
// Created by jeanhua on 2025/8/19.
//

#include "user_interface.h"

UserInterface::~UserInterface() {
    if (mVAO != 0)glDeleteVertexArrays(1,&mVAO);
    if (mVBO != 0)glDeleteBuffers(1,&mVBO);
    delete mShader;
}

UserInterface::UserInterface() {
    this->mShader = new Shader("assets/shader/vertex_ui.glsl","assets/shader/fragment_ui.glsl");
}
