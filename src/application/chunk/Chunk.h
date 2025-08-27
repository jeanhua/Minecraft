//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_CHUNK_H
#define MINECRAFT_CHUNK_H

#include "../../framework/framework.h"
#include "../../framework/shader/shader.h"
#include <array>
#include <vector>
#include <unordered_map>

#define SOLID_SIZE 20.0f
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256

#define AIR 0

#define FULL_SOLID      0b0000010000000000
#define GRASS_STONE     0b0000010000000001
#define LEAVES          0b0000010000000010
#define BOARD_1         0b0000010000000011
#define BOARD_2         0b0000010000000100
#define BRICK           0b0000010000000101
#define DIRT            0b0000010000000110
#define END_STONE       0b0000010000000111
#define CHEST           0b0000010000001000
#define STONE_1         0b0000010000001001
#define STONE_2         0b0000010000001010
#define LOG_OCK         0b0000010000001011
#define MOSS_BLOCK      0b0000010000001100
#define NETHERRACK      0b0000010000001101
#define RESIN_BRICK     0b0000010000001110
#define WOOL_GREEN      0b0000010000001111
#define WOOL_PURPLE     0b0000010000010000
#define WOOL_PINK       0b0000010000010001
#define WOOL_PURPLE1    0b0000010000010010
#define WOOL_ORANGE     0b0000010000010011
#define WOOL_RED        0b0000010000010100
#define WOOL_YELLOW     0b0000010000010101

#define HALF_SOLID      0b0000100000000000
#define WATER           0b0000100000000001


struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

class Chunk {
public:
    explicit Chunk(Shader *shader,int x_id,int z_id);
    ~Chunk();

    void init(const glm::vec3 &position,const std::vector<float>& mapNoise,const std::vector<float>& treeNoise);

    [[nodiscard]] glm::vec3 getPosition()const;
    [[nodiscard]] uint16_t getBlock(int x, int y,int z)const;
    void setBlock(int x, int y, int z, uint16_t block);
    void setModified(bool modified);

    void render(std::unordered_map<std::pair<int,int>,Chunk*,PairHash>& chunks);

private:
    GLuint mSolidVAO, mSolidVBO, mSolidEBO;
    bool needUpdate;

    int x_id,z_id;

    Shader *mShader;
    glm::vec3 initialPosition{0.0f,0.0f,0.0f};

    std::array<std::array<std::array<uint16_t,CHUNK_HEIGHT>,CHUNK_SIZE>,CHUNK_SIZE> mChunk{0};

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    unsigned int indicesCount=0;

    void generateSolidMesh(std::unordered_map<std::pair<int,int>,Chunk*,PairHash>& chunks);

    [[nodiscard]] bool isSolid(std::unordered_map<std::pair<int,int>,Chunk*,PairHash>& chunks,int x, int y, int z) const;

    void addSolidBlockFaces(uint16_t block, int bx, int by, int bz, const bool neighbors[6]);
};

#endif //MINECRAFT_CHUNK_H
