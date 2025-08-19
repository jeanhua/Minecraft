//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_APPLICATION_H
#define MINECRAFT_APPLICATION_H

#include <future>
#include <random>
#include <unordered_map>
#include <utility>
#include <FastNoise/FastNoise.h>
#include <shared_mutex>

#include "../core.h"
#include "../framework/shader/shader.h"
#include "../framework/texture/Texture2D.h"
#include "../framework/texture/textureCube.h"
#include "../framework/camera/camera.h"
#include "skybox/skybox.h"
#include "chunk/Chunk.h"

#define MODEL_SCALE 0.05f

#define CHUNK_RADIUS 8
#define CHUNK_DIAMETER 17

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};


struct ChunkBuffer {
    std::mutex mutex;
    bool isRunning = false;
    std::unordered_map<std::pair<int,int>,Chunk*,PairHash> chunks;
};

struct ChunkAction {
    int chunk_x;
    int chunk_z;
    Chunk* chunk;
};

class Application {
public:
    ~Application();
    static Application* create();
    void init(uint32_t width=800, uint32_t height=600, uint16_t fps=120);
    void run() const;
private:
    static Application* mInstance;
    Application()=default;
    GLFWwindow* mWindow=nullptr;

    Shader* mWorldShader = nullptr;
    Texture2D* mWorldTexture = nullptr;
    Camera* mCamera = nullptr;

    Shader* mSkyboxShader = nullptr;
    TextureCube* mSkyboxTexture = nullptr;
    Skybox* mSkybox = nullptr;

    uint16_t mFPS = 120;

    // map
    std::unordered_map<std::pair<int,int>,Chunk*,PairHash> mChunks;
    Chunk* getChunk(int x_id,int z_id);
    void generateChunks(int x_id,int z_id,std::vector<Chunk *> aroundChunk) const;
    void writeChunk(int x_id,int z_id,Chunk* chunk);
    void removeChunk(int x_id,int z_id);

    ChunkBuffer* mChunkBuffer = nullptr;
    std::future<void> mChunkBufferFuture;

    // generate
    int mapSeed = 0;
    int treeSeed = 0;
    FastNoise::SmartNode<FastNoise::Perlin> perlin;
    FastNoise::SmartNode<FastNoise::FractalFBm> fractal;

private:
    static void frameBufferCallBack(GLFWwindow* window, int width, int height);
    static void keyBoardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallBack(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallBack(GLFWwindow* window, double xpos, double ypos);

    void chunkUpdate();

private:
    static void update(GLFWwindow* window);
};

#endif //MINECRAFT_APPLICATION_H