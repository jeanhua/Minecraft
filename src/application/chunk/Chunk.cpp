//
// Created by jeanhua on 2025/8/16.
//

#include "Chunk.h"
#include "../../utils/utils.h"

glm::vec3 Chunk::getPosition()const {
    return initialPosition;
}

Chunk::~Chunk() {
    if (mVAO!=0) {
        glDeleteVertexArrays(1, &mVAO);
    }
    if (mVBO!=0) {
        glDeleteBuffers(1, &mVBO);
    }
    if (mEBO!=0) {
        glDeleteBuffers(1, &mEBO);
    }
}


std::vector<glm::vec2> getUVPosition(uint16_t block) {
    glm::vec2 top, bottom, left, right, front, back;
    float ps = 1.0f/5.0f;
    switch (block) {
        case GRASS_STONE:
            {
            top = glm::vec2(2.0f*ps, 4.0f*ps);
            bottom = glm::vec2(0.0f, 4.0f*ps);
            left = glm::vec2(1.0f*ps, 4.0f*ps);
            right = glm::vec2(1.0f*ps, 4.0f*ps);
            front = glm::vec2(1.0f*ps, 4.0f*ps);
            back = glm::vec2(1.0f*ps, 4.0f*ps);
            break;
        }
        case LEAVES:
        {
            top = glm::vec2(2.0f*ps, 1.0f*ps);
            bottom = glm::vec2(2.0f*ps, 1.0f*ps);
            left = glm::vec2(2.0f*ps, 1.0f*ps);
            right = glm::vec2(2.0f*ps, 1.0f*ps);
            front = glm::vec2(2.0f*ps, 1.0f*ps);
            back = glm::vec2(2.0f*ps, 1.0f*ps);
            break;
        }
        case BOARD_1:
        {
            top = glm::vec2(3.0f*ps, 3.0f*ps);
            bottom = glm::vec2(3.0f*ps, 3.0f*ps);
            left = glm::vec2(3.0f*ps, 3.0f*ps);
            right = glm::vec2(3.0f*ps, 3.0f*ps);
            front = glm::vec2(3.0f*ps, 3.0f*ps);
            back = glm::vec2(3.0f*ps, 3.0f*ps);
            break;
        }
        case BOARD_2:
        {
            top = glm::vec2(3.0f*ps, 1.0f*ps);
            bottom = glm::vec2(3.0f*ps, 1.0f*ps);
            left = glm::vec2(3.0f*ps, 1.0f*ps);
            right = glm::vec2(3.0f*ps, 1.0f*ps);
            front = glm::vec2(3.0f*ps, 1.0f*ps);
            back = glm::vec2(3.0f*ps, 1.0f*ps);
            break;
        }
        case BRICK:
        {
            top = glm::vec2(4.0f*ps, 1.0f*ps);
            bottom = glm::vec2(4.0f*ps, 1.0f*ps);
            left = glm::vec2(4.0f*ps, 1.0f*ps);
            right = glm::vec2(4.0f*ps, 1.0f*ps);
            front = glm::vec2(4.0f*ps, 1.0f*ps);
            back = glm::vec2(4.0f*ps, 1.0f*ps);
            break;
        }
        case DIRT:
        {
            top = glm::vec2(0.0f*ps, 4.0f*ps);
            bottom = glm::vec2(0.0f*ps, 4.0f*ps);
            left = glm::vec2(0.0f*ps, 4.0f*ps);
            right = glm::vec2(0.0f*ps, 4.0f*ps);
            front = glm::vec2(0.0f*ps, 4.0f*ps);
            back = glm::vec2(0.0f*ps, 4.0f*ps);
            break;
        }
        case END_STONE:
        {
            top = glm::vec2(3.0f*ps, 4.0f*ps);
            bottom = glm::vec2(3.0f*ps, 4.0f*ps);
            left = glm::vec2(3.0f*ps, 4.0f*ps);
            right = glm::vec2(3.0f*ps, 4.0f*ps);
            front = glm::vec2(3.0f*ps, 4.0f*ps);
            back = glm::vec2(3.0f*ps, 4.0f*ps);
            break;
        }
        case CHEST:
        {
            top = glm::vec2(2.0f*ps, 0.0f*ps);
            bottom = glm::vec2(2.0f*ps, 0.0f*ps);
            left = glm::vec2(1.0f*ps, 0.0f*ps);
            right = glm::vec2(1.0f*ps, 0.0f*ps);
            front = glm::vec2(0.0f*ps, 0.0f*ps);
            back = glm::vec2(1.0f*ps, 0.0f*ps);
            break;
        }
        case STONE_1:
        {
            top = glm::vec2(3.0f*ps, 0.0f*ps);
            bottom = glm::vec2(3.0f*ps, 0.0f*ps);
            left = glm::vec2(3.0f*ps, 0.0f*ps);
            right = glm::vec2(3.0f*ps, 0.0f*ps);
            front = glm::vec2(3.0f*ps, 0.0f*ps);
            back = glm::vec2(3.0f*ps, 0.0f*ps);
            break;
        }
        case STONE_2:
        {
            top = glm::vec2(4.0f*ps, 0.0f*ps);
            bottom = glm::vec2(4.0f*ps, 0.0f*ps);
            left = glm::vec2(4.0f*ps, 0.0f*ps);
            right = glm::vec2(4.0f*ps, 0.0f*ps);
            front = glm::vec2(4.0f*ps, 0.0f*ps);
            back = glm::vec2(4.0f*ps, 0.0f*ps);
            break;
        }
        case LOG_OCK:
        {
            top = glm::vec2(0.0f*ps, 3.0f*ps);
            bottom = glm::vec2(0.0f*ps, 3.0f*ps);
            left = glm::vec2(4.0f*ps, 4.0f*ps);
            right = glm::vec2(4.0f*ps, 4.0f*ps);
            front = glm::vec2(4.0f*ps, 4.0f*ps);
            back = glm::vec2(4.0f*ps, 4.0f*ps);
            break;
        }
        case MOSS_BLOCK:
        {
            top = glm::vec2(1.0f*ps, 3.0f*ps);
            bottom = glm::vec2(1.0f*ps, 3.0f*ps);
            left = glm::vec2(1.0f*ps, 3.0f*ps);
            right = glm::vec2(1.0f*ps, 3.0f*ps);
            front = glm::vec2(1.0f*ps, 3.0f*ps);
            back = glm::vec2(1.0f*ps, 3.0f*ps);
            break;
        }
        case NETHERRACK:
        {
            top = glm::vec2(2.0f*ps, 3.0f*ps);
            bottom = glm::vec2(2.0f*ps, 3.0f*ps);
            left = glm::vec2(2.0f*ps, 3.0f*ps);
            right = glm::vec2(2.0f*ps, 3.0f*ps);
            front = glm::vec2(2.0f*ps, 3.0f*ps);
            back = glm::vec2(2.0f*ps, 3.0f*ps);
            break;
        }
        case RESIN_BRICK:
        {
            top = glm::vec2(4.0f*ps, 3.0f*ps);
            bottom = glm::vec2(4.0f*ps, 3.0f*ps);
            left = glm::vec2(4.0f*ps, 3.0f*ps);
            right = glm::vec2(4.0f*ps, 3.0f*ps);
            front = glm::vec2(4.0f*ps, 3.0f*ps);
            back = glm::vec2(4.0f*ps, 3.0f*ps);
            break;
        }
        case WOOL_1:
        {
            top = glm::vec2(0.0f*ps, 2.0f*ps);
            bottom = glm::vec2(0.0f*ps, 2.0f*ps);
            left = glm::vec2(0.0f*ps, 2.0f*ps);
            right = glm::vec2(0.0f*ps, 2.0f*ps);
            front = glm::vec2(0.0f*ps, 2.0f*ps);
            back = glm::vec2(0.0f*ps, 2.0f*ps);
            break;
        }
        case WOOL_2:
        {
            top = glm::vec2(1.0f*ps, 2.0f*ps);
            bottom = glm::vec2(1.0f*ps, 2.0f*ps);
            left = glm::vec2(1.0f*ps, 2.0f*ps);
            right = glm::vec2(1.0f*ps, 2.0f*ps);
            front = glm::vec2(1.0f*ps, 2.0f*ps);
            back = glm::vec2(1.0f*ps, 2.0f*ps);
            break;
        }
        case WOOL_3:
        {
            top = glm::vec2(2.0f*ps, 2.0f*ps);
            bottom = glm::vec2(2.0f*ps, 2.0f*ps);
            left = glm::vec2(2.0f*ps, 2.0f*ps);
            right = glm::vec2(2.0f*ps, 2.0f*ps);
            front = glm::vec2(2.0f*ps, 2.0f*ps);
            back = glm::vec2(2.0f*ps, 2.0f*ps);
            break;
        }
        case WOOL_4:
        {
            top = glm::vec2(3.0f*ps, 2.0f*ps);
            bottom = glm::vec2(3.0f*ps, 2.0f*ps);
            left = glm::vec2(3.0f*ps, 2.0f*ps);
            right = glm::vec2(3.0f*ps, 2.0f*ps);
            front = glm::vec2(3.0f*ps, 2.0f*ps);
            back = glm::vec2(3.0f*ps, 2.0f*ps);
            break;
        }
        case WOOL_5:
        {
            top = glm::vec2(4.0f*ps, 2.0f*ps);
            bottom = glm::vec2(4.0f*ps, 2.0f*ps);
            left = glm::vec2(4.0f*ps, 2.0f*ps);
            right = glm::vec2(4.0f*ps, 2.0f*ps);
            front = glm::vec2(4.0f*ps, 2.0f*ps);
            back = glm::vec2(4.0f*ps, 2.0f*ps);
            break;
        }
        case WOOL_6:
        {
            top = glm::vec2(0.0f*ps, 1.0f*ps);
            bottom = glm::vec2(0.0f*ps, 1.0f*ps);
            left = glm::vec2(0.0f*ps, 1.0f*ps);
            right = glm::vec2(0.0f*ps, 1.0f*ps);
            front = glm::vec2(0.0f*ps, 1.0f*ps);
            back = glm::vec2(0.0f*ps, 1.0f*ps);
            break;
        }
        case WOOL_7:
        {
            top = glm::vec2(1.0f*ps, 1.0f*ps);
            bottom = glm::vec2(1.0f*ps, 1.0f*ps);
            left = glm::vec2(1.0f*ps, 1.0f*ps);
            right = glm::vec2(1.0f*ps, 1.0f*ps);
            front = glm::vec2(1.0f*ps, 1.0f*ps);
            back = glm::vec2(1.0f*ps, 1.0f*ps);
            break;
        }
        default: {
            top = glm::vec2(2.0f*ps, 4.0f*ps);
            bottom = glm::vec2(0.0f, 4.0f*ps);
            left = glm::vec2(1.0f*ps, 4.0f*ps);
            right = glm::vec2(1.0f*ps, 4.0f*ps);
            front = glm::vec2(1.0f*ps, 4.0f*ps);
            back = glm::vec2(1.0f*ps, 4.0f*ps);
        }
    }
    std::vector<glm::vec2> positions;
    positions.insert(positions.end(),{top,bottom,left,right,front,back});
    return positions;
}


void Chunk::init(const glm::vec3 &position,const std::vector<float>& mapNoise,const std::vector<float>& treeNoise) {
    initialPosition = position;
    // test
    for (uint16_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint16_t j = 0; j < CHUNK_SIZE; j++) {
            float noiseValue = mapNoise[i*CHUNK_SIZE + j];
            float normalizedNoise = (noiseValue+1)/4.0f;
            int terrainHeight = static_cast<int>(normalizedNoise * CHUNK_HEIGHT);

            for (uint16_t k = 0; k < CHUNK_HEIGHT; k++) {
                // world
                if (k<terrainHeight) {
                    mChunk[i][j][k] = DIRT;
                }else if (k==terrainHeight && normalizedNoise>0.16) {
                    mChunk[i][j][k] = GRASS_STONE;
                }else if (k==terrainHeight) {
                    mChunk[i][j][k] = DIRT;
                }
            }

            // tree
            float treeValue = treeNoise[i*CHUNK_SIZE + j];
            float normalizedTreeValue = (treeValue+1)/2.0f; // 0~1
            if (normalizedTreeValue>=0.7f) {
                if (i>=5&&i<=CHUNK_SIZE-5&&j>=5&&j<=CHUNK_SIZE-5&&terrainHeight<CHUNK_HEIGHT-5) {
                    mChunk[i][j][terrainHeight] = DIRT;
                    mChunk[i][j][terrainHeight+1] = LOG_OCK;
                    mChunk[i][j][terrainHeight+2] = LOG_OCK;

                    mChunk[i-1][j-1][terrainHeight+3] = LEAVES;
                    mChunk[i-1][j][terrainHeight+3] = LEAVES;
                    mChunk[i-1][j+1][terrainHeight+3] = LEAVES;
                    mChunk[i][j-1][terrainHeight+3] = LEAVES;
                    mChunk[i][j][terrainHeight+3] = LEAVES;
                    mChunk[i][j+1][terrainHeight+3] = LEAVES;
                    mChunk[i+1][j-1][terrainHeight+3] = LEAVES;
                    mChunk[i+1][j][terrainHeight+3] = LEAVES;
                    mChunk[i+1][j+1][terrainHeight+3] = LEAVES;

                    mChunk[i-1][j-1][terrainHeight+4] = LEAVES;
                    mChunk[i-1][j][terrainHeight+4] = LEAVES;
                    mChunk[i-1][j+1][terrainHeight+4] = LEAVES;
                    mChunk[i][j-1][terrainHeight+4] = LEAVES;
                    mChunk[i][j][terrainHeight+4] = LEAVES;
                    mChunk[i][j+1][terrainHeight+4] = LEAVES;
                    mChunk[i+1][j-1][terrainHeight+4] = LEAVES;
                    mChunk[i+1][j][terrainHeight+4] = LEAVES;
                    mChunk[i+1][j+1][terrainHeight+4] = LEAVES;
                }
            }
        }
    }
}


void Chunk::render() {
    if (needUpdate)generateMesh();
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Chunk::Chunk(Shader *shader) {
    mVAO = mVBO = mEBO = 0;
    needUpdate = true;
    mShader = shader;

}

bool Chunk::isSolid(const uint16_t x, const uint16_t y, const uint16_t z) const {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_HEIGHT) {
        return false;
    }
    return (mChunk[x][y][z]&FULL_SOLID)!=0;
}

void Chunk::generateMesh() {
    vertices.clear();
    indices.clear();

    for (uint16_t i = 0; i < CHUNK_SIZE; i++) {
        for (uint16_t j = 0; j < CHUNK_SIZE; j++) {
            for (uint16_t k = 0; k < CHUNK_HEIGHT; k++) {
                if (mChunk[i][j][k] == AIR) continue;

                bool neighbors[6] = {
                    isSolid(i, j, k + 1), // top
                    isSolid(i, j, k - 1), // bottom
                    isSolid(i + 1, j, k), // front
                    isSolid(i - 1, j, k), // back
                    isSolid(i, j + 1, k), // right
                    isSolid(i, j - 1, k) // left
                };

                addBlockFaces(mChunk[i][j][k],i, j, k, neighbors);
            }
        }
    }


    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
        mVAO = 0;
    }
    if (mVBO != 0) {
        glDeleteBuffers(1, &mVBO);
        mVBO = 0;
    }
    if (mEBO != 0) {
        glDeleteBuffers(1, &mEBO);
        mEBO = 0;
    }

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex)), vertices.data(),GL_STATIC_DRAW);

    glEnableVertexAttribArray(mShader->getAttribPos("aPosition"));
    glVertexAttribPointer(mShader->getAttribPos("aPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, x));

    glEnableVertexAttribArray(mShader->getAttribPos("aNormal"));
    glVertexAttribPointer(mShader->getAttribPos("aNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, nx));

    glEnableVertexAttribArray(mShader->getAttribPos("aUV"));
    glVertexAttribPointer(mShader->getAttribPos("aUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, u));

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(GLuint)), indices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertices.clear();
    needUpdate = false;
}

void Chunk::addBlockFaces(uint16_t block,int bx, int by, int bz, const bool neighbors[6]) {
    glm::vec3 blockPos = glm::vec3(static_cast<float>(by) * SOLID_SIZE, static_cast<float>(bz) * SOLID_SIZE,
                                   static_cast<float>(bx) * SOLID_SIZE);

    blockPos += initialPosition;

    const float half_size = SOLID_SIZE / 2.0f;

    const auto uvPosition = getUVPosition(block);
    const float ps = 1.0f/5.0f;

    // generate top
    if (!neighbors[0]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 1, 0, uvPosition[0].x, uvPosition[0].y};
        Vertex v2 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 1, 0, uvPosition[0].x+ps, uvPosition[0].y};
        Vertex v3 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 1, 0, uvPosition[0].x+ps, uvPosition[0].y+ps};
        Vertex v4 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 1, 0, uvPosition[0].x, uvPosition[0].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // bottom
    if (!neighbors[1]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, uvPosition[1].x, uvPosition[1].y};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, -1, 0, uvPosition[1].x+ps, uvPosition[1].y};
        Vertex v3 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, uvPosition[1].x+ps, uvPosition[1].y+ps};
        Vertex v4 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, -1, 0, uvPosition[1].x, uvPosition[1].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // front
    if (!neighbors[2]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x, uvPosition[4].y};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x+ps, uvPosition[4].y};
        Vertex v3 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x+ps, uvPosition[4].y+ps};
        Vertex v4 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, 0, 0, 1, uvPosition[4].x, uvPosition[4].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // back
    if (!neighbors[3]) {
        Vertex v1 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x, uvPosition[5].y};
        Vertex v2 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x+ps, uvPosition[5].y};
        Vertex v3 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x+ps, uvPosition[5].y+ps};
        Vertex v4 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 0, 0, -1, uvPosition[5].x, uvPosition[5].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // right
    if (!neighbors[4]) {
        Vertex v1 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z + half_size, 1, 0, 0, uvPosition[3].x, uvPosition[3].y};
        Vertex v2 = {blockPos.x + half_size, blockPos.y - half_size, blockPos.z - half_size, 1, 0, 0, uvPosition[3].x+ps, uvPosition[3].y};
        Vertex v3 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z - half_size, 1, 0, 0, uvPosition[3].x+ps, uvPosition[3].y+ps};
        Vertex v4 = {blockPos.x + half_size, blockPos.y + half_size, blockPos.z + half_size, 1, 0, 0, uvPosition[3].x, uvPosition[3].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }

    // left
    if (!neighbors[5]) {
        Vertex v1 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z - half_size, -1, 0, 0, uvPosition[2].x, uvPosition[2].y};
        Vertex v2 = {blockPos.x - half_size, blockPos.y - half_size, blockPos.z + half_size, -1, 0, 0, uvPosition[2].x+ps, uvPosition[2].y};
        Vertex v3 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z + half_size, -1, 0, 0, uvPosition[2].x+ps, uvPosition[2].y+ps};
        Vertex v4 = {blockPos.x - half_size, blockPos.y + half_size, blockPos.z - half_size, -1, 0, 0, uvPosition[2].x, uvPosition[2].y+ps};
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        uint32_t base = vertices.size() - 4;
        indices.insert(indices.end(), {
                           base, base + 1, base + 2,
                           base, base + 2, base + 3
                       });
    }
}
