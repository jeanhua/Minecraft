//
// Created by jeanhua on 2025/8/21.
//

#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include "../../framework/shader/shader.h"
#include "../../framework/texture/texture2D.h"
#include "../../framework/texture/textureCube.h"
#include "../../framework/camera/camera.h"
#include "../skybox/skybox.h"
#include "../chunk/Chunk.h"
#include "../task/render_task.hpp"

#include <unordered_map>
#include <FastNoise/SmartNode.h>
#include <FastNoise/Generators/Fractal.h>
#include <FastNoise/Generators/Perlin.h>

#define MAX_GEN_CHUNK_THREAD 10

#define MODEL_SCALE 0.05f

#define CHUNK_RADIUS 10
#define CHUNK_DIAMETER 21
#define MAX_GEN_CHUNK_THREAD 10

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};


class world {
public:
    world();
    ~world();

    void setAspectRatio(float radio) const;

    void render(GLFWwindow* window);

private:
    Shader* mWorldShader = nullptr;
    Texture2D* mWorldTexture = nullptr;
    Camera* mCamera = nullptr;

    Shader* mSkyboxShader = nullptr;
    TextureCube* mSkyboxTexture = nullptr;
    Skybox* mSkybox = nullptr;

    // map
    bool initial = false;
    int currentCenterX=0, currentCenterZ=0;
    std::unordered_map<std::pair<int,int>,Chunk*,PairHash> mChunks;
    Chunk* getChunk(int x_id,int z_id);
    void chunkUpdate();
    std::vector<ChunkAction>chunkBuffer;
    void generateMissingChunks(const std::vector<std::pair<int, int>>& missingChunks);
    void writeChunk(int x_id,int z_id,Chunk* chunk);
    void removeChunk(int x_id,int z_id);

    // render pool
    int waitingChunks=0;
    ThreadPool<ChunkAction,RenderParam>* renderPool=nullptr;

    // generate
    int mapSeed = 0;
    int treeSeed = 0;
    FastNoise::SmartNode<FastNoise::Perlin> perlin;
    FastNoise::SmartNode<FastNoise::FractalFBm> fractal;
};


#endif //MINECRAFT_WORLD_H