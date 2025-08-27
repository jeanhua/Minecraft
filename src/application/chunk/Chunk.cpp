//
// Created by jeanhua on 2025/8/16.
//

#include "Chunk.h"

glm::vec3 Chunk::getPosition() const {
    return initialPosition;
}

Chunk::~Chunk() {
    if (mSolidVAO != 0) {
        glDeleteVertexArrays(1, &mSolidVAO);
    }
    if (mSolidVBO != 0) {
        glDeleteBuffers(1, &mSolidVBO);
    }
    if (mSolidEBO != 0) {
        glDeleteBuffers(1, &mSolidEBO);
    }
    if (mWaterVAO != 0) {
        glDeleteVertexArrays(1, &mWaterVAO);
    }
    if (mWaterVBO != 0) {
        glDeleteBuffers(1, &mWaterVBO);
    }
    if (mWaterEBO != 0) {
        glDeleteBuffers(1, &mWaterEBO);
    }
}


std::vector<glm::vec2> getUVPosition(uint16_t block) {
    glm::vec2 top, bottom, left, right, front, back;
    float ps = 1.0f / 5.0f;
    switch (block) {
        case GRASS_STONE: {
            top = glm::vec2(2.0f * ps, 4.0f * ps);
            bottom = glm::vec2(0.0f, 4.0f * ps);
            left = glm::vec2(1.0f * ps, 4.0f * ps);
            right = glm::vec2(1.0f * ps, 4.0f * ps);
            front = glm::vec2(1.0f * ps, 4.0f * ps);
            back = glm::vec2(1.0f * ps, 4.0f * ps);
            break;
        }
        case LEAVES: {
            top = glm::vec2(2.0f * ps, 1.0f * ps);
            bottom = glm::vec2(2.0f * ps, 1.0f * ps);
            left = glm::vec2(2.0f * ps, 1.0f * ps);
            right = glm::vec2(2.0f * ps, 1.0f * ps);
            front = glm::vec2(2.0f * ps, 1.0f * ps);
            back = glm::vec2(2.0f * ps, 1.0f * ps);
            break;
        }
        case BOARD_1: {
            top = glm::vec2(3.0f * ps, 3.0f * ps);
            bottom = glm::vec2(3.0f * ps, 3.0f * ps);
            left = glm::vec2(3.0f * ps, 3.0f * ps);
            right = glm::vec2(3.0f * ps, 3.0f * ps);
            front = glm::vec2(3.0f * ps, 3.0f * ps);
            back = glm::vec2(3.0f * ps, 3.0f * ps);
            break;
        }
        case BOARD_2: {
            top = glm::vec2(3.0f * ps, 1.0f * ps);
            bottom = glm::vec2(3.0f * ps, 1.0f * ps);
            left = glm::vec2(3.0f * ps, 1.0f * ps);
            right = glm::vec2(3.0f * ps, 1.0f * ps);
            front = glm::vec2(3.0f * ps, 1.0f * ps);
            back = glm::vec2(3.0f * ps, 1.0f * ps);
            break;
        }
        case BRICK: {
            top = glm::vec2(4.0f * ps, 1.0f * ps);
            bottom = glm::vec2(4.0f * ps, 1.0f * ps);
            left = glm::vec2(4.0f * ps, 1.0f * ps);
            right = glm::vec2(4.0f * ps, 1.0f * ps);
            front = glm::vec2(4.0f * ps, 1.0f * ps);
            back = glm::vec2(4.0f * ps, 1.0f * ps);
            break;
        }
        case DIRT: {
            top = glm::vec2(0.0f * ps, 4.0f * ps);
            bottom = glm::vec2(0.0f * ps, 4.0f * ps);
            left = glm::vec2(0.0f * ps, 4.0f * ps);
            right = glm::vec2(0.0f * ps, 4.0f * ps);
            front = glm::vec2(0.0f * ps, 4.0f * ps);
            back = glm::vec2(0.0f * ps, 4.0f * ps);
            break;
        }
        case END_STONE: {
            top = glm::vec2(3.0f * ps, 4.0f * ps);
            bottom = glm::vec2(3.0f * ps, 4.0f * ps);
            left = glm::vec2(3.0f * ps, 4.0f * ps);
            right = glm::vec2(3.0f * ps, 4.0f * ps);
            front = glm::vec2(3.0f * ps, 4.0f * ps);
            back = glm::vec2(3.0f * ps, 4.0f * ps);
            break;
        }
        case CHEST: {
            top = glm::vec2(2.0f * ps, 0.0f * ps);
            bottom = glm::vec2(2.0f * ps, 0.0f * ps);
            left = glm::vec2(1.0f * ps, 0.0f * ps);
            right = glm::vec2(1.0f * ps, 0.0f * ps);
            front = glm::vec2(0.0f * ps, 0.0f * ps);
            back = glm::vec2(1.0f * ps, 0.0f * ps);
            break;
        }
        case STONE_1: {
            top = glm::vec2(3.0f * ps, 0.0f * ps);
            bottom = glm::vec2(3.0f * ps, 0.0f * ps);
            left = glm::vec2(3.0f * ps, 0.0f * ps);
            right = glm::vec2(3.0f * ps, 0.0f * ps);
            front = glm::vec2(3.0f * ps, 0.0f * ps);
            back = glm::vec2(3.0f * ps, 0.0f * ps);
            break;
        }
        case STONE_2: {
            top = glm::vec2(4.0f * ps, 0.0f * ps);
            bottom = glm::vec2(4.0f * ps, 0.0f * ps);
            left = glm::vec2(4.0f * ps, 0.0f * ps);
            right = glm::vec2(4.0f * ps, 0.0f * ps);
            front = glm::vec2(4.0f * ps, 0.0f * ps);
            back = glm::vec2(4.0f * ps, 0.0f * ps);
            break;
        }
        case LOG_OCK: {
            top = glm::vec2(0.0f * ps, 3.0f * ps);
            bottom = glm::vec2(0.0f * ps, 3.0f * ps);
            left = glm::vec2(4.0f * ps, 4.0f * ps);
            right = glm::vec2(4.0f * ps, 4.0f * ps);
            front = glm::vec2(4.0f * ps, 4.0f * ps);
            back = glm::vec2(4.0f * ps, 4.0f * ps);
            break;
        }
        case MOSS_BLOCK: {
            top = glm::vec2(1.0f * ps, 3.0f * ps);
            bottom = glm::vec2(1.0f * ps, 3.0f * ps);
            left = glm::vec2(1.0f * ps, 3.0f * ps);
            right = glm::vec2(1.0f * ps, 3.0f * ps);
            front = glm::vec2(1.0f * ps, 3.0f * ps);
            back = glm::vec2(1.0f * ps, 3.0f * ps);
            break;
        }
        case NETHERRACK: {
            top = glm::vec2(2.0f * ps, 3.0f * ps);
            bottom = glm::vec2(2.0f * ps, 3.0f * ps);
            left = glm::vec2(2.0f * ps, 3.0f * ps);
            right = glm::vec2(2.0f * ps, 3.0f * ps);
            front = glm::vec2(2.0f * ps, 3.0f * ps);
            back = glm::vec2(2.0f * ps, 3.0f * ps);
            break;
        }
        case RESIN_BRICK: {
            top = glm::vec2(4.0f * ps, 3.0f * ps);
            bottom = glm::vec2(4.0f * ps, 3.0f * ps);
            left = glm::vec2(4.0f * ps, 3.0f * ps);
            right = glm::vec2(4.0f * ps, 3.0f * ps);
            front = glm::vec2(4.0f * ps, 3.0f * ps);
            back = glm::vec2(4.0f * ps, 3.0f * ps);
            break;
        }
        case WOOL_GREEN: {
            top = glm::vec2(0.0f * ps, 2.0f * ps);
            bottom = glm::vec2(0.0f * ps, 2.0f * ps);
            left = glm::vec2(0.0f * ps, 2.0f * ps);
            right = glm::vec2(0.0f * ps, 2.0f * ps);
            front = glm::vec2(0.0f * ps, 2.0f * ps);
            back = glm::vec2(0.0f * ps, 2.0f * ps);
            break;
        }
        case WOOL_PURPLE: {
            top = glm::vec2(1.0f * ps, 2.0f * ps);
            bottom = glm::vec2(1.0f * ps, 2.0f * ps);
            left = glm::vec2(1.0f * ps, 2.0f * ps);
            right = glm::vec2(1.0f * ps, 2.0f * ps);
            front = glm::vec2(1.0f * ps, 2.0f * ps);
            back = glm::vec2(1.0f * ps, 2.0f * ps);
            break;
        }
        case WOOL_PINK: {
            top = glm::vec2(2.0f * ps, 2.0f * ps);
            bottom = glm::vec2(2.0f * ps, 2.0f * ps);
            left = glm::vec2(2.0f * ps, 2.0f * ps);
            right = glm::vec2(2.0f * ps, 2.0f * ps);
            front = glm::vec2(2.0f * ps, 2.0f * ps);
            back = glm::vec2(2.0f * ps, 2.0f * ps);
            break;
        }
        case WOOL_PURPLE1: {
            top = glm::vec2(3.0f * ps, 2.0f * ps);
            bottom = glm::vec2(3.0f * ps, 2.0f * ps);
            left = glm::vec2(3.0f * ps, 2.0f * ps);
            right = glm::vec2(3.0f * ps, 2.0f * ps);
            front = glm::vec2(3.0f * ps, 2.0f * ps);
            back = glm::vec2(3.0f * ps, 2.0f * ps);
            break;
        }
        case WOOL_ORANGE: {
            top = glm::vec2(4.0f * ps, 2.0f * ps);
            bottom = glm::vec2(4.0f * ps, 2.0f * ps);
            left = glm::vec2(4.0f * ps, 2.0f * ps);
            right = glm::vec2(4.0f * ps, 2.0f * ps);
            front = glm::vec2(4.0f * ps, 2.0f * ps);
            back = glm::vec2(4.0f * ps, 2.0f * ps);
            break;
        }
        case WOOL_RED: {
            top = glm::vec2(0.0f * ps, 1.0f * ps);
            bottom = glm::vec2(0.0f * ps, 1.0f * ps);
            left = glm::vec2(0.0f * ps, 1.0f * ps);
            right = glm::vec2(0.0f * ps, 1.0f * ps);
            front = glm::vec2(0.0f * ps, 1.0f * ps);
            back = glm::vec2(0.0f * ps, 1.0f * ps);
            break;
        }
        case WOOL_YELLOW: {
            top = glm::vec2(1.0f * ps, 1.0f * ps);
            bottom = glm::vec2(1.0f * ps, 1.0f * ps);
            left = glm::vec2(1.0f * ps, 1.0f * ps);
            right = glm::vec2(1.0f * ps, 1.0f * ps);
            front = glm::vec2(1.0f * ps, 1.0f * ps);
            back = glm::vec2(1.0f * ps, 1.0f * ps);
            break;
        }
        default: {
            top = glm::vec2(2.0f * ps, 4.0f * ps);
            bottom = glm::vec2(0.0f, 4.0f * ps);
            left = glm::vec2(1.0f * ps, 4.0f * ps);
            right = glm::vec2(1.0f * ps, 4.0f * ps);
            front = glm::vec2(1.0f * ps, 4.0f * ps);
            back = glm::vec2(1.0f * ps, 4.0f * ps);
        }
    }
    std::vector<glm::vec2> positions;
    positions.insert(positions.end(), {top, bottom, left, right, front, back});
    return positions;
}


void Chunk::init(const glm::vec3 &position, const std::vector<float> &mapNoise, const std::vector<float> &treeNoise) {
    initialPosition = position;
    for (uint16_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint16_t j = 0; j < CHUNK_SIZE; j++) {
            float noiseValue = mapNoise[i * CHUNK_SIZE + j];
            float normalizedNoise = (noiseValue + 1) / 4.0f;
            int terrainHeight = static_cast<int>(normalizedNoise * CHUNK_HEIGHT);
            constexpr int waterLine = 50;
            for (uint16_t k = 0; k < CHUNK_HEIGHT; k++) {
                // world
                if (k < terrainHeight) {
                    mChunk[i][j][k] = DIRT;
                } else if (k == terrainHeight) {
                    if (terrainHeight < waterLine) {
                        mChunk[i][j][k] = DIRT;
                    }else {
                        mChunk[i][j][k] = GRASS_STONE;
                    }
                }
                // water
                if (k > terrainHeight && k <= waterLine && mChunk[i][j][k] == AIR) {
                    mChunk[i][j][k] = WATER;
                }
            }

            // tree
            float treeValue = treeNoise[i * CHUNK_SIZE + j];
            float normalizedTreeValue = (treeValue + 1) / 2.0f; // 0~1
            if (normalizedTreeValue >= 0.7f && normalizedTreeValue <= 0.95f) {
                if (i >= 5 && i <= CHUNK_SIZE - 5 && j >= 5 && j <= CHUNK_SIZE - 5 && terrainHeight < CHUNK_HEIGHT -
                    5 && terrainHeight > waterLine) {
                    mChunk[i][j][terrainHeight] = DIRT;
                    mChunk[i][j][terrainHeight + 1] = LOG_OCK;
                    mChunk[i][j][terrainHeight + 2] = LOG_OCK;

                    mChunk[i - 1][j - 1][terrainHeight + 3] = LEAVES;
                    mChunk[i - 1][j][terrainHeight + 3] = LEAVES;
                    mChunk[i - 1][j + 1][terrainHeight + 3] = LEAVES;
                    mChunk[i][j - 1][terrainHeight + 3] = LEAVES;
                    mChunk[i][j][terrainHeight + 3] = LEAVES;
                    mChunk[i][j + 1][terrainHeight + 3] = LEAVES;
                    mChunk[i + 1][j - 1][terrainHeight + 3] = LEAVES;
                    mChunk[i + 1][j][terrainHeight + 3] = LEAVES;
                    mChunk[i + 1][j + 1][terrainHeight + 3] = LEAVES;

                    mChunk[i - 1][j - 1][terrainHeight + 4] = LEAVES;
                    mChunk[i - 1][j][terrainHeight + 4] = LEAVES;
                    mChunk[i - 1][j + 1][terrainHeight + 4] = LEAVES;
                    mChunk[i][j - 1][terrainHeight + 4] = LEAVES;
                    mChunk[i][j][terrainHeight + 4] = LEAVES;
                    mChunk[i][j + 1][terrainHeight + 4] = LEAVES;
                    mChunk[i + 1][j - 1][terrainHeight + 4] = LEAVES;
                    mChunk[i + 1][j][terrainHeight + 4] = LEAVES;
                    mChunk[i + 1][j + 1][terrainHeight + 4] = LEAVES;
                }
            }
        }
    }
}


void Chunk::renderSolid(std::unordered_map<std::pair<int, int>, Chunk *, PairHash> &chunks) {
    if (needUpdate) {
        generateMesh(chunks);
    }
    if (solidIndicesCount != 0) {
        mWorldShader->begin();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glBindVertexArray(mSolidVAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(solidIndicesCount), GL_UNSIGNED_INT, nullptr);
    }
}

void Chunk::renderWater() const {
    if (waterIndicesCount != 0) {
        mWaterShader->begin();
        glEnable(GL_DEPTH_TEST);
        //glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(mWaterVAO);
        glDrawElements(GL_TRIANGLES, static_cast<int>(waterIndicesCount), GL_UNSIGNED_INT, nullptr);
        glDisable(GL_BLEND);
        //glDepthMask(GL_TRUE);
    }
}

Chunk::Chunk(Shader *mWorldShader, Shader *mWaterShader, int x_id, int z_id) {
    mSolidVAO = mSolidVBO = mSolidEBO = 0;
    mWaterVAO = mWaterVBO = mWaterEBO = 0;
    needUpdate = true;
    this->mWorldShader = mWorldShader;
    this->mWaterShader = mWaterShader;
    this->x_id = x_id;
    this->z_id = z_id;
}

bool Chunk::isSolid(std::unordered_map<std::pair<int, int>, Chunk *, PairHash> &chunks, int x, int y, int z,
                    bool includeWater) const {
    if (z < 0 || z >= CHUNK_HEIGHT) {
        return z < 0;
    }

    if (x < 0) {
        auto another = chunks.find({x_id, z_id - 1});
        if (another == chunks.end()) return true;
        return another->second->isSolid(chunks,CHUNK_SIZE - 1, y, z, includeWater);
    }
    if (x >= CHUNK_SIZE) {
        auto another = chunks.find({x_id, z_id + 1});
        if (another == chunks.end()) return true;
        return another->second->isSolid(chunks, 0, y, z, includeWater);
    }
    if (y < 0) {
        auto another = chunks.find({x_id - 1, z_id});
        if (another == chunks.end()) return true;
        return another->second->isSolid(chunks, x,CHUNK_SIZE - 1, z, includeWater);
    }
    if (y >= CHUNK_SIZE) {
        auto another = chunks.find({x_id + 1, z_id});
        if (another == chunks.end()) return true;
        return another->second->isSolid(chunks, x, 0, z, includeWater);
    }

    if (includeWater) {
        return (mChunk[x][y][z] & FULL_SOLID) != 0 || mChunk[x][y][z] == WATER;
    }
    return (mChunk[x][y][z] & FULL_SOLID) != 0;
}

bool Chunk::isWater(std::unordered_map<std::pair<int, int>, Chunk *, PairHash> &chunks, int x, int y, int z) const {
    if (z < 0 || z >= CHUNK_HEIGHT) {
        return z < 0;
    }

    if (x < 0) {
        auto another = chunks.find({x_id, z_id - 1});
        if (another == chunks.end()) return true;
        return another->second->isWater(chunks,CHUNK_SIZE - 1, y, z);
    }
    if (x >= CHUNK_SIZE) {
        auto another = chunks.find({x_id, z_id + 1});
        if (another == chunks.end()) return true;
        return another->second->isWater(chunks, 0, y, z);
    }
    if (y < 0) {
        auto another = chunks.find({x_id - 1, z_id});
        if (another == chunks.end()) return true;
        return another->second->isWater(chunks, x,CHUNK_SIZE - 1, z);
    }
    if (y >= CHUNK_SIZE) {
        auto another = chunks.find({x_id + 1, z_id});
        if (another == chunks.end()) return true;
        return another->second->isWater(chunks, x, 0, z);
    }

    return mChunk[x][y][z]==WATER;
}


void Chunk::generateMesh(std::unordered_map<std::pair<int, int>, Chunk *, PairHash> &chunks) {
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_HEIGHT; k++) {
                if ((mChunk[i][j][k] & FULL_SOLID) == 0 || mChunk[i][j][k] == WATER) continue;
                bool neighbors[6] = {
                    isSolid(chunks, i, j, k + 1), // top
                    isSolid(chunks, i, j, k - 1), // bottom
                    isSolid(chunks, i + 1, j, k), // front
                    isSolid(chunks, i - 1, j, k), // back
                    isSolid(chunks, i, j + 1, k), // right
                    isSolid(chunks, i, j - 1, k) // left
                };

                addSolidBlockFaces(mChunk[i][j][k], i, j, k, neighbors);
            }
        }
    }

    if (mSolidVAO != 0) {
        glDeleteVertexArrays(1, &mSolidVAO);
        mSolidVAO = 0;
    }
    if (mSolidVBO != 0) {
        glDeleteBuffers(1, &mSolidVBO);
        mSolidVBO = 0;
    }
    if (mSolidEBO != 0) {
        glDeleteBuffers(1, &mSolidEBO);
        mSolidEBO = 0;
    }

    glGenVertexArrays(1, &mSolidVAO);
    glBindVertexArray(mSolidVAO);

    glGenBuffers(1, &mSolidVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mSolidVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex)), vertices.data(),GL_STATIC_DRAW);

    glEnableVertexAttribArray(mWorldShader->getAttribPos("aPosition"));
    glVertexAttribPointer(mWorldShader->getAttribPos("aPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, x)));

    glEnableVertexAttribArray(mWorldShader->getAttribPos("aNormal"));
    glVertexAttribPointer(mWorldShader->getAttribPos("aNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, nx)));

    glEnableVertexAttribArray(mWorldShader->getAttribPos("aUV"));
    glVertexAttribPointer(mWorldShader->getAttribPos("aUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, u)));

    glGenBuffers(1, &mSolidEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSolidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(GLuint)), indices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    solidIndicesCount = indices.size();
    vertices.clear();
    indices.clear();

    if (mWaterVAO != 0) {
        glDeleteVertexArrays(1, &mWaterVAO);
        mWaterVAO = 0;
    }
    if (mWaterVBO != 0) {
        glDeleteBuffers(1, &mWaterVBO);
        mWaterVBO = 0;
    }
    if (mWaterEBO != 0) {
        glDeleteBuffers(1, &mWaterEBO);
        mWaterEBO = 0;
    }

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_HEIGHT; k++) {
                if (mChunk[i][j][k] != WATER) continue;
                bool neighbors[6] = {
                    isWater(chunks, i, j, k + 1), // top
                    isSolid(chunks, i, j, k - 1, true), // bottom
                    isSolid(chunks, i + 1, j, k, true), // front
                    isSolid(chunks, i - 1, j, k, true), // back
                    isSolid(chunks, i, j + 1, k, true), // right
                    isSolid(chunks, i, j - 1, k, true) // left
                };

                addWaterBlockFaces(i, j, k, neighbors);
            }
        }
    }

    glGenVertexArrays(1, &mWaterVAO);
    glBindVertexArray(mWaterVAO);

    glGenBuffers(1, &mWaterVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mWaterVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex)), vertices.data(),GL_STATIC_DRAW);


    glEnableVertexAttribArray(mWaterShader->getAttribPos("aPosition"));
    glVertexAttribPointer(mWaterShader->getAttribPos("aPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, x)));

    glGenBuffers(1, &mWaterEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWaterEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(GLuint)), indices.data(),
                 GL_STATIC_DRAW);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    waterIndicesCount = indices.size();

    vertices.clear();
    indices.clear();
    needUpdate = false;
}

void Chunk::addSolidBlockFaces(uint16_t block, int bx, int by, int bz, const bool neighbors[6]) {
    glm::vec3 blockPos = glm::vec3(static_cast<float>(by) * SOLID_SIZE, static_cast<float>(bz) * SOLID_SIZE,
                                   static_cast<float>(bx) * SOLID_SIZE);

    blockPos += initialPosition;

    const float half_size = SOLID_SIZE / 2.0f;

    const auto uvPosition = getUVPosition(block);
    const float ps = 1.0f / 5.0f;

    // generate top
    if (!neighbors[0]) {
        Vertex v1 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 1, 0, uvPosition[0].x,
            uvPosition[0].y
        };
        Vertex v2 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 1, 0, uvPosition[0].x + ps,
            uvPosition[0].y
        };
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 1, 0, uvPosition[0].x + ps,
            uvPosition[0].y + ps
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 1, 0, uvPosition[0].x,
            uvPosition[0].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // bottom
    if (!neighbors[1]) {
        Vertex v1 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, uvPosition[1].x,
            uvPosition[1].y
        };
        Vertex v2 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, uvPosition[1].x + ps,
            uvPosition[1].y
        };
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, uvPosition[1].x + ps,
            uvPosition[1].y + ps
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, uvPosition[1].x,
            uvPosition[1].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // front
    if (!neighbors[2]) {
        Vertex v1 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x,
            uvPosition[4].y
        };
        Vertex v2 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x + ps,
            uvPosition[4].y
        };
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x + ps,
            uvPosition[4].y + ps
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x,
            uvPosition[4].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // back
    if (!neighbors[3]) {
        Vertex v1 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x,
            uvPosition[5].y
        };
        Vertex v2 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x + ps,
            uvPosition[5].y
        };
        Vertex v3 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x + ps,
            uvPosition[5].y + ps
        };
        Vertex v4 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x,
            uvPosition[5].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // right
    if (!neighbors[4]) {
        Vertex v1 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 1, 0, 0, uvPosition[3].x,
            uvPosition[3].y
        };
        Vertex v2 = {
            blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 1, 0, 0, uvPosition[3].x + ps,
            uvPosition[3].y
        };
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 1, 0, 0, uvPosition[3].x + ps,
            uvPosition[3].y + ps
        };
        Vertex v4 = {
            blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 1, 0, 0, uvPosition[3].x,
            uvPosition[3].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // left
    if (!neighbors[5]) {
        Vertex v1 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, -1, 0, 0, uvPosition[2].x,
            uvPosition[2].y
        };
        Vertex v2 = {
            blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, -1, 0, 0, uvPosition[2].x + ps,
            uvPosition[2].y
        };
        Vertex v3 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, -1, 0, 0, uvPosition[2].x + ps,
            uvPosition[2].y + ps
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, -1, 0, 0, uvPosition[2].x,
            uvPosition[2].y + ps
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }
}

uint16_t Chunk::getBlock(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_HEIGHT) return 0;
    return mChunk[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, const uint16_t block) {
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_HEIGHT) return;
    mChunk[x][y][z] = block;
}

void Chunk::setModified(bool modified) {
    needUpdate = modified;
}


void Chunk::addWaterBlockFaces(int bx, int by, int bz, const bool neighbors[6]) {
    glm::vec3 blockPos = glm::vec3(static_cast<float>(by) * SOLID_SIZE, static_cast<float>(bz) * SOLID_SIZE,
                                   static_cast<float>(bx) * SOLID_SIZE);

    blockPos += initialPosition;
    const float half_size = SOLID_SIZE / 2.0f;

    constexpr float waterOffset = SOLID_SIZE / 10.0f;

    // generate top
    if (!neighbors[0]) {
        Vertex v1 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, 0, 1, 0, 0, 0
        };
        Vertex v2 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, 0, 1, 0, 0, 0
        };
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, 0, 1, 0, 0, 0
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, 0, 1, 0, 0, 0
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // bottom
    if (!neighbors[1]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, 0, 0};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, 0, 0};
        Vertex v3 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, 0, 0};
        Vertex v4 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, 0, 0};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // front
    if (!neighbors[2]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, 0, 0};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, 0, 0};
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, 0, 0, 1, 0, 0
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, 0, 0, 1, 0, 0
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // back
    if (!neighbors[3]) {
        Vertex v1 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, 0, 0};
        Vertex v2 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, 0, 0};
        Vertex v3 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, 0, 0, -1, 0, 0
        };
        Vertex v4 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, 0, 0, -1, 0, 0
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // right
    if (!neighbors[4]) {
        Vertex v1 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 1, 0, 0, 0, 0};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 1, 0, 0, 0, 0};
        Vertex v3 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, 1, 0, 0, 0, 0
        };
        Vertex v4 = {
            blockPos.x + half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, 1, 0, 0, 0, 0
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // left
    if (!neighbors[5]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, -1, 0, 0, 0, 0};
        Vertex v2 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, -1, 0, 0, 0, 0};
        Vertex v3 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z + half_size, -1, 0, 0, 0, 0
        };
        Vertex v4 = {
            blockPos.x - half_size, blockPos.y + half_size - waterOffset, blockPos.z - half_size, -1, 0, 0, 0, 0
        };
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }
}
