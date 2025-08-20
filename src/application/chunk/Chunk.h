//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

#include "../../core.h"
#include "../../framework/shader/shader.h"
#include <array>
#include <vector>

#define SOLID_SIZE 20.0f
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256

#define AIR 0

#define FULL_SOLID  0b0000010000000000
#define GRASS_STONE 0b0000010000000001
#define LEAVES      0b0000010000000010
#define BOARD_1     0b0000010000000011
#define BOARD_2     0b0000010000000100
#define BRICK       0b0000010000000101
#define DIRT        0b0000010000000110
#define END_STONE   0b0000010000000111
#define CHEST       0b0000010000001000
#define STONE_1     0b0000010000001001
#define STONE_2     0b0000010000001010
#define LOG_OCK     0b0000010000001011
#define MOSS_BLOCK  0b0000010000001100
#define NETHERRACK  0b0000010000001101
#define RESIN_BRICK 0b0000010000001110
#define WOOL_1      0b0000010000001111
#define WOOL_2      0b0000010000010000
#define WOOL_3      0b0000010000010001
#define WOOL_4      0b0000010000010010
#define WOOL_5      0b0000010000010011
#define WOOL_6      0b0000010000010100
#define WOOL_7      0b0000010000010101

#define HALF_SOLID  0b0000100000000000

using Vertex = struct {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

class Chunk {
public:
    explicit Chunk(Shader *shader);

    ~Chunk();

    void init(const glm::vec3 &position,const std::vector<float>& mapNoise,const std::vector<float>& treeNoise);

    [[nodiscard]] glm::vec3 getPosition()const;

    void render();

private:
    GLuint mVAO, mVBO, mEBO;
    bool needUpdate;

    Shader *mShader;
    glm::vec3 initialPosition{0.0f,0.0f,0.0f};

    std::array<std::array<std::array<uint16_t,CHUNK_HEIGHT>,CHUNK_SIZE>,CHUNK_SIZE> mChunk{0};

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    void generateMesh();

    [[nodiscard]] bool isSolid(uint16_t x, uint16_t y, uint16_t z) const;

    void addBlockFaces(uint16_t block, int bx, int by, int bz, const bool neighbors[6]);

    std::vector<uint16_t> chunkHeight;
};

#endif //MINECRAFT_CHUNK_H
