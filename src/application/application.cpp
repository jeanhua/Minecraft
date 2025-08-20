//
// Created by jeanhua on 2025/8/16.
//

#include "application.h"

#include <chrono>
#include <cmath>
#include <set>
#include <shared_mutex>

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
    renderPool->stop();
    delete renderPool;
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
    glfwSetInputMode(mWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "error when load glad loader" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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

    // thread pool
    renderPool = new ThreadPool<ChunkAction, RenderParam>();
    renderPool->init(MAX_GEN_CHUNK_THREAD);
}

void Application::run() const {
    double lastTime = glfwGetTime();
    double lastTitleUpdate = lastTime;
    const double frameTime = 1.0 / mFPS;
    const double titleUpdateInterval = 0.5; // 每0.5秒更新一次标题

    int frameCount = 0;
    double accumulatedTime = 0.0;

    while (!glfwWindowShouldClose(mWindow)) {
        double frameStartTime = glfwGetTime();
        double deltaTime = frameStartTime - lastTime;

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update(mWindow);
        glfwSwapBuffers(mWindow);

        frameCount++;
        accumulatedTime += deltaTime;

        if (frameStartTime - lastTitleUpdate >= titleUpdateInterval) {
            double avgFPS = frameCount / accumulatedTime;
            std::string title = "Minecraft FPS: " + std::to_string(static_cast<int>(avgFPS + 0.5));
            glfwSetWindowTitle(mWindow, title.c_str());

            frameCount = 0;
            accumulatedTime = 0.0;
            lastTitleUpdate = frameStartTime;
        }
        while (glfwGetTime() - frameStartTime < frameTime) {
        }
        lastTime = frameStartTime;
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
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // destroy block
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        // put block
    }
}

void Application::keyBoardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }
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

    // environment
    app->mWorldShader->setFloat("fogEnd", 250.0f);

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

    if (initial==false) {
        initial=true;
        currentCenterX = 114514;
        currentCenterZ = 114514;
    }

    std::set<std::pair<int, int> > requiredChunks; // 用于跟踪需要的区块

    for (int i = x_id; i < x_id + CHUNK_DIAMETER; i++) {
        for (int k = z_id; k < z_id + CHUNK_DIAMETER; k++) {
            requiredChunks.insert({i, k});
        }
    }

    if (waitingChunks == 0&&(std::abs(x_id+CHUNK_RADIUS-currentCenterX)>2||std::abs(z_id+CHUNK_RADIUS-currentCenterZ)>2)) {
        std::vector<std::pair<int, int> > missingChunks;
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
            generateMissingChunks(missingChunks);
        }
    }


    // get output
    auto finishedChunks = renderPool->getOutput();
    if (!finishedChunks.empty()) {
        chunkBuffer.insert(chunkBuffer.end(), finishedChunks.begin(), finishedChunks.end());
    }

    if (!chunkBuffer.empty()) {
        auto chunk = chunkBuffer.back();
        chunkBuffer.pop_back();
        writeChunk(chunk.chunk_x, chunk.chunk_z, chunk.chunk);
        waitingChunks--;
        currentCenterX = x_id+CHUNK_RADIUS;
        currentCenterZ = z_id+CHUNK_RADIUS;

        // clear
        std::vector<std::pair<int, int> > chunksToRemove;
        for (const auto &pair: mChunks) {
            if (requiredChunks.find(pair.first) == requiredChunks.end()) {
                chunksToRemove.push_back(pair.first);
            }
        }
        for (const auto &key: chunksToRemove) {
            removeChunk(key.first, key.second);
        }
    }

    for (auto chunk: mChunks) {
        if (chunk.second != nullptr) {
            chunk.second->render();
        }
    }
}

void Application::generateMissingChunks(const std::vector<std::pair<int, int> > &missingChunks) {
    for (const auto &chunkCoord: missingChunks) {
        waitingChunks++;
        renderPool->addTask(std::make_shared<render_task>(render_task(RenderParam{
            chunkCoord.first, chunkCoord.second,
            fractal, mWorldShader, mapSeed, treeSeed
        })));
    }
}
