//
// Created by jeanhua on 2025/8/21.
//

#include "world.h"

#include <chrono>
#include <set>

#include "../global_status.h"
#include "../../utils/utils.h"

glm::mat4 getModelPosition(glm::vec3 pos, float scale) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f) * scale);
    return model;
}

world::world() {
    std::cout << "Start to compile world shader program..." << std::endl;
    mWorldShader = new Shader("assets/shader/vertex_world.glsl", "assets/shader/fragment_world.glsl");
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

    // user interface
    userInterface = new UserInterface(mWorldTexture);
}

world::~world() {
    renderPool->stop();
    delete renderPool;
    delete mWorldShader;
    delete mWorldTexture;
    delete mCamera;
    delete mSkyboxShader;
    delete mSkyboxTexture;
    delete mSkybox;
    delete userInterface;
    for (auto chunk: mChunks) {
        delete chunk.second;
    }
}

Chunk *world::getChunk(int x_id, int z_id) {
    const auto pair = std::make_pair(x_id, z_id);
    const auto it = mChunks.find(pair);
    return it == mChunks.end() ? nullptr : it->second;
}

void world::removeChunk(int x_id, int z_id) {
    if (mChunks.contains(std::make_pair(x_id, z_id))) {
        delete mChunks[std::make_pair(x_id, z_id)];
        mChunks.erase(std::make_pair(x_id, z_id));
    }
}

void world::writeChunk(int x_id, int z_id, Chunk *chunk) {
    if (mChunks.contains(std::make_pair(x_id, z_id))) {
        delete mChunks[std::make_pair(x_id, z_id)];
        mChunks.erase(std::make_pair(x_id, z_id));
    }
    mChunks.insert({std::make_pair(x_id, z_id), chunk});
    noticeAroundChunk(x_id, z_id);
}

void world::generateMissingChunks(const std::vector<std::pair<int, int> > &missingChunks) {
    std::vector<std::shared_ptr<Task<ChunkAction,RenderParam>>> preRenderTasks;
    for (const auto &[fst, snd]: missingChunks) {
        waitingChunks++;
        auto task = render_task(RenderParam{
            fst, snd,
            fractal, mWorldShader, mapSeed, treeSeed
        });
        preRenderTasks.push_back(std::make_shared<render_task>(task));
    }
    renderPool->addTask(std::move(preRenderTasks));
}

void world::setAspectRatio(float radio) const {
    mCamera->setAspectRatio(radio);
}


void world::render(GLFWwindow* window) {
    // skybox
    if (userInterface->showSkybox) {
        mSkyboxTexture->bind();
        mSkyboxShader->begin();
        mSkyboxShader->setInt("skyboxSampler", 0);
        mSkyboxShader->setMat4("projection", mCamera->getProjectionMatrix());
        mSkyboxShader->setMat4("view", glm::mat4(glm::mat3(mCamera->getViewMatrix())));
        mSkybox->render();
        Shader::end();
    }

    // world
    mWorldTexture->bind();
    mWorldShader->begin();
    mWorldShader->setInt("world_sampler", 0);

    // initial global model matrix
    mWorldShader->setMat4("transform", getModelPosition(glm::vec3(0.0f, 0.0f, 0.0f),MODEL_SCALE));

    // environment
    mWorldShader->setBool("showFog",userInterface->showFog);
    mWorldShader->setBool("showSunshine",userInterface->showSunshine);
    mWorldShader->setFloat("ambientStrength",userInterface->ambientStrength);
    mWorldShader->setFloat("specularStrength",userInterface->specularStrength);
    mWorldShader->setInt("shininess",userInterface->shininess);
    mWorldShader->setFloat("fogEnd", 250.0f);

    // camera
    mCamera->onUpdate(window, *mWorldShader);

    // render chunk
    chunkUpdate();

    Shader::end();

    // user interface
    userInterface->render();
}

void world::chunkUpdate() {
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
            chunk.second->render(mChunks);
        }
    }
}


uint16_t world::getBlock(int Chunk_X_ID, int Chunk_Z_ID, int x, int y, int z) {
    auto chunk = getChunk(Chunk_X_ID,Chunk_Z_ID);
    if (chunk == nullptr) {
        return 0;
    }
    return chunk->getBlock(x, y, z);
}

void world::noticeAroundChunk(int x_id, int z_id) {
    const auto left = getChunk(x_id-1, z_id);
    const auto right = getChunk(x_id+1, z_id);
    const auto front  = getChunk(x_id, z_id+1);
    const auto back   = getChunk(x_id, z_id-1);
    if (left!=nullptr)left->setModified(true);
    if (right!=nullptr)right->setModified(true);
    if (front!=nullptr)front->setModified(true);
    if (back!=nullptr)back->setModified(true);
}

struct VT {
    int x,y,z;
};

int ftoi(float value) {
    return static_cast<int>(std::floor(value));
}

int safeMod(int a, int b) {
    int result = a % b;
    return result < 0 ? result + b : result;
}

void world::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (global_status::isUIShow)return;
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        bool destroy = (button == GLFW_MOUSE_BUTTON_LEFT);
        auto cameraPosition = mCamera->getPosition() / MODEL_SCALE;
        auto cameraFront = mCamera->getFront();
        glm::vec3 rayDirection = glm::normalize(cameraFront);

        constexpr float maxRayLength = 10.0f * SOLID_SIZE;
        constexpr float stepLength = 0.01f * SOLID_SIZE;

        float rayLength = 0.1f;
        VT lastValidBlock{-1, -1, -1};
        bool hit = false;

        while (rayLength <= maxRayLength && !hit) {
            glm::vec3 currentPos = cameraPosition + rayLength * rayDirection;

            int blockX = ftoi(currentPos.x / SOLID_SIZE);
            int blockY = ftoi(currentPos.y / SOLID_SIZE);
            int blockZ = ftoi(currentPos.z / SOLID_SIZE);

            if (blockY < 0 || blockY >= CHUNK_HEIGHT) {
                rayLength += stepLength;
                continue;
            }

            int chunkX = ftoi(static_cast<float>(blockX) / CHUNK_SIZE);
            int chunkZ = ftoi(static_cast<float>(blockZ) / CHUNK_SIZE);

            auto chunk = getChunk(chunkX, chunkZ);
            if (chunk == nullptr) {
                rayLength += stepLength;
                continue;
            }

            int x_id = safeMod(blockX, CHUNK_SIZE);
            int z_id = safeMod(blockZ, CHUNK_SIZE);

            auto noticeEdge = [=](int x,int z,int currentChunkX,int currentChunkZ)->void {
                // left
                if (x==0) {
                    auto another = getChunk(currentChunkX-1, currentChunkZ);
                    if (another != nullptr) {
                        printf("notice left\n");
                        another->setModified(true);
                    }
                }
                // right
                else if (x==CHUNK_SIZE-1) {
                    auto another = getChunk(currentChunkX+1, currentChunkZ);
                    if (another != nullptr) {
                        another->setModified(true);
                    }
                }
                // back
                if (z==0) {
                    auto another = getChunk(currentChunkX, currentChunkZ-1);
                    if (another != nullptr) {
                        another->setModified(true);
                    }
                }
                // front
                else if (z==CHUNK_SIZE-1) {
                    auto another = getChunk(currentChunkX, currentChunkZ+1);
                    if (another != nullptr) {
                        another->setModified(true);
                    }
                }
            };

            if (chunk->getBlock(z_id, x_id, blockY) != AIR) {
                if (destroy) {
                    chunk->setBlock(z_id, x_id, blockY, AIR);
                    chunk->setModified(true);
                    hit = true;
                    noticeEdge(x_id,z_id,chunkX,chunkZ);
                } else {
                    if (lastValidBlock.x != -1) {
                        int lastChunkX = ftoi(static_cast<float>(lastValidBlock.x) / CHUNK_SIZE);
                        int lastChunkZ = ftoi(static_cast<float>(lastValidBlock.z) / CHUNK_SIZE);
                        auto lastChunk = getChunk(lastChunkX, lastChunkZ);

                        if (lastChunk != nullptr && lastValidBlock.y >= 0 && lastValidBlock.y < CHUNK_HEIGHT) {
                            int last_x_id = safeMod(lastValidBlock.x, CHUNK_SIZE);
                            int last_z_id = safeMod(lastValidBlock.z, CHUNK_SIZE);

                            if (lastChunk->getBlock(last_z_id, last_x_id, lastValidBlock.y) == AIR) {
                                glm::vec3 playerPos = cameraPosition / SOLID_SIZE;
                                int playerBlockX = ftoi(playerPos.x);
                                int playerBlockY = ftoi(playerPos.y);
                                int playerBlockZ = ftoi(playerPos.z);

                                bool isPlayerPosition =
                                    (lastValidBlock.x == playerBlockX && lastValidBlock.z == playerBlockZ &&
                                     (lastValidBlock.y == playerBlockY || lastValidBlock.y == playerBlockY + 1));

                                if (!isPlayerPosition) {
                                    lastChunk->setBlock(last_z_id, last_x_id, lastValidBlock.y, static_cast<uint8_t>(userInterface->currentBlock)+GRASS_STONE);
                                    lastChunk->setModified(true);
                                    noticeEdge(last_x_id, last_z_id, lastChunkX, lastChunkZ);
                                }
                            }
                        }
                    }
                    hit = true;
                }
                break;
            }

            lastValidBlock.x = blockX;
            lastValidBlock.y = blockY;
            lastValidBlock.z = blockZ;

            rayLength += stepLength;
        }
    }
}
