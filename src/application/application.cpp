//
// Created by jeanhua on 2025/8/16.
//

#include "application.h"

#include <chrono>
#include <cmath>
#include <thread>
#include <semaphore>

#include "../framework/framework.h"
#include "../utils/utils.h"

Application *Application::mInstance = nullptr;

//GLuint vao=0;

Application *Application::create() {
    if (mInstance == nullptr) {
        mInstance = new Application();
    }
    return mInstance;
}

Application::~Application() {
    delete mWorldShader;
    delete mWorldTexture;
    delete mCamera;
    delete mSkyboxShader;
    delete mSkyboxTexture;
    delete mSkybox;
    delete mChunkBuffer;
    for (auto chunk: mChunks) {
        delete chunk.second;
    }
}

void Application::init(uint32_t width, uint32_t height, uint16_t fps) {
    mFPS = fps;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    mWindow = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Minecraft", nullptr, nullptr);
    if (mWindow == nullptr) {
        std::cout << "error when create window" << std::endl;
        return;
    }

    glfwMakeContextCurrent(mWindow);

    glfwSetWindowUserPointer(mWindow, this);
    glfwSetFramebufferSizeCallback(mWindow, frameBufferCallBack);
    glfwSetKeyCallback(mWindow, keyBoardCallBack);
    glfwSetMouseButtonCallback(mWindow, mouseCallBack);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "error when load glad loader" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::cout << "Start to compile world shader program..." << std::endl;
    mWorldShader = new Shader("assets/shader/vertex.glsl", "assets/shader/fragment.glsl");
    if (mWorldShader->getShaderProgram() != 0)std::cout << "Compile shader program success." << std::endl;
    mWorldTexture = new Texture2D("assets/texture/textures.png", 0);
    mCamera = new Camera(4.0f / 3.0f, "projectionMatrix", "viewMatrix",MODEL_SCALE);

    std::cout << "Start to compile skybox shader program..." << std::endl;
    mSkyboxShader = new Shader("assets/shader/vertex_skybox.glsl", "assets/shader/fragment_skybox.glsl");
    if (mSkyboxShader->getShaderProgram() != 0)std::cout << "Compile shader program success." << std::endl;
    std::vector<std::string> paths = {
        "assets/texture/skybox/xpos.png",
        "assets/texture/skybox/xneg.png",
        "assets/texture/skybox/ypos.png",
        "assets/texture/skybox/yneg.png",
        "assets/texture/skybox/zpos.png",
        "assets/texture/skybox/zneg.png",
    };
    mSkyboxTexture = new TextureCube(paths, 0);
    mSkybox = new Skybox(mSkyboxShader);

    //vao = createCubeVertexArray(mShader->getAttribPos("aPosition"),mShader->getAttribPos("aNormal"),mShader->getAttribPos("aUV"));


    // perlin func
    mapSeed = generateRandomNumber(1, 745323228);
    treeSeed = generateRandomNumber(4234, 23453453);
    perlin = FastNoise::New<FastNoise::Perlin>();
    fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource(perlin);
    fractal->SetOctaveCount(5);
    fractal->SetGain(0.5f);
    fractal->SetLacunarity(2.0f);

    // chunkBuffer
    mChunkBuffer = new ChunkBuffer();
}

void Application::run() const {
    double lastTime = glfwGetTime();
    const double frameTime = 1.0 / mFPS;

    while (!glfwWindowShouldClose(mWindow)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // FPS control
        if (deltaTime < frameTime) {
            double sleepTime = (frameTime - deltaTime) * 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleepTime)));
        }

        // Frame processing
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update(mWindow);
        glfwSwapBuffers(mWindow);
    }
    glfwTerminate();
    std::cout << "Game over, bye!" << std::endl;
}

void Application::frameBufferCallBack(GLFWwindow *window, int width, int height) {
    const auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    app->mCamera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void Application::cursorPosCallBack(GLFWwindow *window, double xpos, double ypos) {
}

void Application::mouseCallBack(GLFWwindow *window, int button, int action, int mods) {
}

void Application::keyBoardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
}

glm::mat4 getModelPosition(glm::vec3 pos, float scale) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f) * scale);
    return model;
}


void Application::update(GLFWwindow *window) {
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));

    // skybox
    app->mSkyboxTexture->bind();
    app->mSkyboxShader->begin();
    app->mSkyboxShader->setInt("skybox", 0);
    app->mSkyboxShader->setMat4("projection", app->mCamera->getProjectionMatrix());
    app->mSkyboxShader->setMat4("view", glm::mat4(glm::mat3(app->mCamera->getViewMatrix())));
    app->mSkybox->render();
    Shader::end();


    // world
    app->mWorldTexture->bind();
    app->mWorldShader->begin();
    app->mWorldShader->setInt("sampler", 0);

    // initial global model matrix
    app->mWorldShader->setMat4("transform", getModelPosition(glm::vec3(0.0f, -10.0f, 0.0f),MODEL_SCALE));

    // fog
    app->mWorldShader->setFloat("fogDensity", 0.005f);
    app->mWorldShader->setVec3("fogColor", 1.0f, 1.0f, 1.0f);

    // camera
    app->mCamera->onUpdate(window, *app->mWorldShader);

    // render chunk
    app->chunkUpdate();

    Shader::end();
}

Chunk *Application::getChunk(int x_id, int z_id) {
    const auto pair = std::make_pair(x_id, z_id);
    const auto it = mChunks.find(pair);
    return it == mChunks.end() ? nullptr : it->second;
}

void Application::writeChunk(int x_id, int z_id, Chunk *chunk) {
    if (mChunks.contains(std::make_pair(x_id, z_id))) {
        delete mChunks[std::make_pair(x_id, z_id)];
        mChunks.erase(std::make_pair(x_id, z_id));
    }
    mChunks.insert({std::make_pair(x_id, z_id), chunk});
}

void Application::removeChunk(int x_id, int z_id) {
    if (mChunks.contains(std::make_pair(x_id, z_id))) {
        delete mChunks[std::make_pair(x_id, z_id)];
        mChunks.erase(std::make_pair(x_id, z_id));
    }
}

void Application::chunkUpdate() {
    glm::vec3 cameraPos = mCamera->getPosition();
    cameraPos /= MODEL_SCALE;

    float aChunkSize = SOLID_SIZE * CHUNK_SIZE;
    int x_id = static_cast<int>(std::floor(cameraPos.x / aChunkSize)) - CHUNK_RADIUS;
    int z_id = static_cast<int>(std::floor(cameraPos.z / aChunkSize)) - CHUNK_RADIUS;

    std::vector<Chunk *> aroundChunk;
    std::set<std::pair<int, int>> requiredChunks; // 用于跟踪需要的区块
    aroundChunk.reserve(CHUNK_DIAMETER * CHUNK_DIAMETER);

    for (int i = x_id; i < x_id + CHUNK_DIAMETER; i++) {
        for (int k = z_id; k < z_id + CHUNK_DIAMETER; k++) {
            requiredChunks.insert({i, k});
            auto chunk = getChunk(i, k);
            aroundChunk.push_back(chunk);
        }
    }

    if (mChunkBuffer->mutex.try_lock()) {
        if (mChunkBuffer->isRunning == false) {
            mChunkBuffer->isRunning = true;
            for (auto& pending : mChunkBuffer->chunks) {
                writeChunk(pending.first.first, pending.first.second, pending.second);
            }
            mChunkBuffer->chunks.clear();

            std::vector<std::pair<int, int>> missingChunks;
            for (int i = 0; i < CHUNK_DIAMETER; i++) {
                for (int k = 0; k < CHUNK_DIAMETER; k++) {
                    int chunkX = x_id + i;
                    int chunkZ = z_id + k;
                    if (getChunk(chunkX, chunkZ) == nullptr) {
                        missingChunks.emplace_back(chunkX, chunkZ);
                    }
                }
            }

            if (!missingChunks.empty()) {
                mChunkBufferFuture = std::async(std::launch::async,
                    &Application::generateMissingChunks, this, missingChunks);
            } else {
                mChunkBuffer->isRunning = false;
            }
        }
        mChunkBuffer->mutex.unlock();
    }

    std::vector<std::pair<int, int>> chunksToRemove;
    for (const auto &pair : mChunks) {
        if (requiredChunks.find(pair.first) == requiredChunks.end()) {
            chunksToRemove.push_back(pair.first);
        }
    }

    for (const auto &key : chunksToRemove) {
        removeChunk(key.first, key.second);
    }

    for (auto chunk : aroundChunk) {
        if (chunk != nullptr) {
            chunk->render();
        }
    }
}

void Application::generateMissingChunks(const std::vector<std::pair<int, int>>& missingChunks) const {
    float aChunkSize = SOLID_SIZE * CHUNK_SIZE;
    std::vector<std::future<ChunkAction>> futures;
    std::counting_semaphore<MAX_GEN_CHUNK_THREAD> semaphore(MAX_GEN_CHUNK_THREAD);

    for (const auto& chunkCoord : missingChunks) {
        semaphore.acquire();
        auto f = std::async(std::launch::async, [&, this, chunkCoord]() -> ChunkAction {
            std::vector<float> worldNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
            std::vector<float> treeNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
            constexpr float frequency = 0.005f;

            worldNoiseValues.assign(CHUNK_SIZE * CHUNK_SIZE, 0.0f);
            treeNoiseValues.assign(CHUNK_SIZE * CHUNK_SIZE, 0.0f);

            const int chunkX = chunkCoord.first;
            const int chunkZ = chunkCoord.second;

            fractal->GenUniformGrid2D(worldNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                      CHUNK_SIZE, CHUNK_SIZE, frequency, mapSeed);
            fractal->GenUniformGrid2D(treeNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                      CHUNK_SIZE, CHUNK_SIZE, 0.5, treeSeed);

            const auto newChunk = new Chunk(mWorldShader);
            glm::vec3 chunkPos = glm::vec3(
                static_cast<float>(chunkX) * aChunkSize,
                0.0f,
                static_cast<float>(chunkZ) * aChunkSize
            );
            newChunk->init(chunkPos, worldNoiseValues, treeNoiseValues);
            semaphore.release();

            return ChunkAction{
                chunkX,
                chunkZ,
                newChunk,
            };
        });
        futures.push_back(std::move(f));
    }

    std::lock_guard lock(mChunkBuffer->mutex);
    for (auto &f : futures) {
        auto action = f.get();
        mChunkBuffer->chunks[std::make_pair(action.chunk_x, action.chunk_z)] = action.chunk;
    }
    mChunkBuffer->isRunning = false;
}