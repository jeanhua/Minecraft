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

#include "../core.h"
#include "../framework/shader/shader.h"
#include "../framework/texture/Texture2D.h"
#include "../framework/texture/textureCube.h"
#include "../framework/camera/camera.h"
#include "skybox/skybox.h"
#include "chunk/Chunk.h"
#include "task/render_task.hpp"

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
    bool initial = false;
    int currentCenterX=0, currentCenterZ=0;
    std::unordered_map<std::pair<int,int>,Chunk*,PairHash> mChunks;
    Chunk* getChunk(int x_id,int z_id);
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