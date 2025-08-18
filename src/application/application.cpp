//
// Created by jeanhua on 2025/8/16.
//

#include "application.h"

#include <chrono>
#include <thread>
#include <cmath>

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
    mCamera = new Camera(4.0f / 3.0f, "vpMat",MODEL_SCALE);

    std::cout<<"Start to compile skybox shader program..."<<std::endl;
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
    mSkyboxTexture = new TextureCube(paths,0);
    mSkybox = new Skybox(mSkyboxShader);

    //vao = createCubeVertexArray(mShader->getAttribPos("aPosition"),mShader->getAttribPos("aNormal"),mShader->getAttribPos("aUV"));


    // perlin func
    mapSeed = generateRandomNumber(1,745323228);
    treeSeed = generateRandomNumber(4234,23453453);
    perlin = FastNoise::New<FastNoise::Perlin>();
    fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource(perlin);
    fractal->SetOctaveCount(5);
    fractal->SetGain(0.5f);
    fractal->SetLacunarity(2.0f);
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
    app->mSkyboxShader->setInt("skybox",0);
    app->mSkyboxShader->setMat4("projection",app->mCamera->getProjectionMatrix());
    app->mSkyboxShader->setMat4("view",glm::mat4(glm::mat3(app->mCamera->getViewMatrix())));
    app->mSkybox->render();
    app->mSkyboxShader->end();


    // world
    app->mWorldTexture->bind();
    app->mWorldShader->begin();
    app->mWorldShader->setInt("sampler", 0);

    // initial global model matrix
    app->mWorldShader->setMat4("posMat", getModelPosition(glm::vec3(-0.0f, -10.0f, 0.0f),MODEL_SCALE));

    // camera
    app->mCamera->onUpdate(window, *app->mWorldShader);

    // render chunk
    app->chunkUpdate();

    app->mWorldShader->end();
}


void Application::chunkUpdate() {
    glm::vec3 cameraPos = mCamera->getPosition();

    cameraPos.x /= MODEL_SCALE;
    cameraPos.y /= MODEL_SCALE;
    cameraPos.z /= MODEL_SCALE;

    float aChunkSize = SOLID_SIZE * CHUNK_SIZE;
    int x_id = static_cast<int>(std::floor(cameraPos.x / aChunkSize)) - 3;
    int z_id = static_cast<int>(std::floor(cameraPos.z / aChunkSize)) - 3;

    std::vector<Chunk *> aroundChunk;
    aroundChunk.reserve(49);
    for (int i = x_id; i < x_id + 7; i++) {
        for (int k = z_id; k < z_id + 7; k++) {
            auto chunk = getChunk(i, k);
            aroundChunk.push_back(chunk);
        }
    }


    for (int index = 0; index < aroundChunk.size(); index++) {
        if (aroundChunk[index] != nullptr)continue;

        constexpr float frequency = 0.01f;
        std::vector<float> worldNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
        std::vector<float> treeNoiseValues(CHUNK_SIZE * CHUNK_SIZE);
        int chunkX = x_id + index / 7;
        int chunkZ = z_id + index % 7;
        fractal->GenUniformGrid2D(worldNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                  CHUNK_SIZE,CHUNK_SIZE, frequency, mapSeed);
        fractal->GenUniformGrid2D(treeNoiseValues.data(), chunkX * CHUNK_SIZE, chunkZ * CHUNK_SIZE,
                                  CHUNK_SIZE,CHUNK_SIZE, 0.5, treeSeed);
        auto newChunk = new Chunk(mWorldShader);
        glm::vec3 chunkPos = glm::vec3(
            static_cast<float>(chunkX) * aChunkSize,
            0.0f,
            static_cast<float>(chunkZ) * aChunkSize
        );
        newChunk->init(chunkPos, worldNoiseValues,treeNoiseValues);
        mChunks.insert({std::make_pair(chunkX, chunkZ), newChunk});
        aroundChunk[index] = newChunk;
    }

    // render
    for (auto chunk: aroundChunk) {
        if (chunk != nullptr) {
            chunk->render();
        }
    }

    // clear far chunk
    std::vector<std::pair<int, int>> chunksToRemove;
    for (const auto& pair : mChunks) {
        int x = pair.first.first;
        int z = pair.first.second;

        if (x < x_id || x >= x_id + 7 ||
            z < z_id || z >= z_id + 7) {
            chunksToRemove.emplace_back(pair.first);
            }
    }
    for (const auto& key : chunksToRemove) {
        auto it = mChunks.find(key);
        if (it != mChunks.end()) {
            delete it->second;
            mChunks.erase(it);
        }
    }
}

Chunk *Application::getChunk(int x_id, int z_id) {
    auto pair = std::make_pair(x_id, z_id);
    auto it = mChunks.find(pair);
    return it == mChunks.end() ? nullptr : it->second;
}
