//
// Created by jeanhua on 2025/8/19.
//

#include "user_interface.h"

UserInterface::~UserInterface() {
    if (mVAO != 0)
        glDeleteVertexArrays(1, &mVAO);
    if (mVBO != 0)
        glDeleteBuffers(1, &mVBO);
    delete mShader;
}

std::pair<float,float>getTextureUV(const uint32_t id) {
    int x_id = static_cast<int>(id%5);
    int y_id = static_cast<int>(id/5);
    return std::make_pair(static_cast<float>(x_id)*0.2f,static_cast<float>(y_id)*0.2f);
}

UserInterface::UserInterface(Texture2D *worldTexture) {
    if (worldTexture==nullptr)std::cout<<"worldTexture is null"<<std::endl;
    std::cout << "Start to compile user interface shader program..." << std::endl;
    this->mShader = new Shader("assets/shader/vertex_ui.glsl", "assets/shader/fragment_ui.glsl");
    if (mShader->getShaderProgram() != 0)std::cout << "Compile shader program success." << std::endl;
    this->worldTexture = worldTexture;

    genBuffers();
}

void UserInterface::render() const {
    mShader->begin();
    glBindVertexArray(mVAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 4);
    Shader::end();
}

void UserInterface::genBuffers() {
    constexpr float size = 0.01f;
    std::vector<float> vertices = {
        -size,  0.0f, 0.0f,
         size,  0.0f, 0.0f,
         0.0f, -size, 0.0f,
         0.0f,  size, 0.0f
    };

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    const uint32_t aPosition = mShader->getAttribPos("aPosition");
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(aPosition);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
