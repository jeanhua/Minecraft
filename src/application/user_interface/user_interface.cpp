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
    worldTexture->bind();
    mShader->begin();
    glDisable(GL_DEPTH_TEST);
    mShader->setInt("world_sampler", 0);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
    Shader::end();
}

void UserInterface::genBuffers() {
    if (mVAO != 0)glDeleteVertexArrays(1,&mVAO);
    if (mVBO != 0)glDeleteBuffers(1,&mVBO);
    if (mEBO != 0)glDeleteBuffers(1,&mEBO);

    uint32_t showTextureID[8]={
        0,3,5,7,11,15,17,20
    };

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    std::vector<VertexSimple> vertices;
    vertices.reserve(32);
    for (int i = 0; i < 8; i++) {
        auto uv = getTextureUV(showTextureID[i]);
        vertices.push_back({
            -1.0f+static_cast<float>(i)*0.25f,-0.85f,0.0f,uv.first,uv.second+0.2f,
        });
        vertices.push_back({
            -1.0f+static_cast<float>(i)*0.25f,-1.0f,0.0f,uv.first,uv.second,
        });
        vertices.push_back({
            -1.0f+static_cast<float>(i+1)*0.25f,-0.85f,0.0f,uv.first+0.2f,uv.second+0.2f,
        });
        vertices.push_back({
            -1.0f+static_cast<float>(i+1)*0.25f,-1.0f,0.0f,uv.first+0.2f,uv.second,
        });
    }

    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size()*sizeof(VertexSimple)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(mShader->getAttribPos("aPosition"));
    glVertexAttribPointer(mShader->getAttribPos("aPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexSimple), reinterpret_cast<void *>(offsetof(VertexSimple, x)));
    glEnableVertexAttribArray(mShader->getAttribPos("aUV"));
    glVertexAttribPointer(mShader->getAttribPos("aUV"), 2, GL_FLOAT, GL_FALSE, sizeof(VertexSimple), reinterpret_cast<void *>(offsetof(VertexSimple, u)));

    std::vector<uint32_t> indices;
    indices.reserve(6*8);

    for (uint32_t i=0;i<8;i++) {
        indices.insert(indices.end(),{
            4*i,4*i+1,4*i+2
        });
        indices.insert(indices.end(),{
            4*i+2,4*i+1,4*i+3
        });
    }
    indicesCount = static_cast<int>(indices.size());

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount*static_cast<int>(sizeof(uint32_t)), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
