//
// Created by jeanhua on 2025/8/20.
//

#ifndef MINECRAFT_RENDER_TASK_H
#define MINECRAFT_RENDER_TASK_H

#include "task.hpp"
#include "../chunk/Chunk.h"
#include <FastNoise/FastNoise.h>

struct RenderParam{
    int chunk_x;
    int chunk_z;
    FastNoise::SmartNode<FastNoise::FractalFBm> fractal;
    Shader *mWorldShader;
    Shader *mWaterShader;
    int mapSeed,treeSeed;
};

struct ChunkAction {
    int chunk_x;
    int chunk_z;
    Chunk* chunk;
};

class render_task final :public Task<ChunkAction,RenderParam>{
public:
    explicit render_task(RenderParam&& inp);
    ChunkAction doWork()override;
    ~render_task() override =default;
};

inline render_task::render_task(RenderParam &&inp) : Task<ChunkAction, RenderParam>(inp) {}


inline ChunkAction render_task::doWork() {
    constexpr float aChunkSize = SOLID_SIZE * CHUNK_SIZE;
    std::vector<float> worldNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
    std::vector<float> treeNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
    constexpr float frequency = 0.005f;

    worldNoiseValues.assign(CHUNK_SIZE * CHUNK_SIZE, 0.0f);
    treeNoiseValues.assign(CHUNK_SIZE * CHUNK_SIZE, 0.0f);

    const int chunkX = this->m_in.chunk_x;
    const int chunkZ = m_in.chunk_z;

    m_in.fractal->GenUniformGrid2D(worldNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                              CHUNK_SIZE, CHUNK_SIZE, frequency, m_in.mapSeed);
    m_in.fractal->GenUniformGrid2D(treeNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                              CHUNK_SIZE, CHUNK_SIZE, 0.5, m_in.treeSeed);

    const auto newChunk = new Chunk(m_in.mWorldShader,m_in.mWaterShader,chunkX,chunkZ);
    glm::vec3 chunkPos = glm::vec3(
        static_cast<float>(chunkX) * aChunkSize,
        0.0f,
        static_cast<float>(chunkZ) * aChunkSize
    );
    newChunk->init(chunkPos, worldNoiseValues, treeNoiseValues);
    return ChunkAction{
        chunkX,
        chunkZ,
        newChunk,
    };
}


#endif //MINECRAFT_RENDER_TASK_H