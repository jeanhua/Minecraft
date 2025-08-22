//
// Created by jeanhua on 2025/8/16.
//

#include "application.h"

#include <chrono>
#include <shared_mutex>

#include "../framework/framework.h"

Application *Application::mInstance = nullptr;

//GLuint vao=0;

Application *Application::create() {
    if (mInstance == nullptr) {
        mInstance = new Application();
    }
    return mInstance;
}

Application::~Application() {
    delete mWorld;
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

    mWorld = new world();
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
    app->mWorld->setAspectRatio(static_cast<float>(width)/static_cast<float>(height));
}

void Application::cursorPosCallBack(GLFWwindow *window, double xpos, double ypos) {
}

void Application::mouseCallBack(GLFWwindow *window, int button, int action, int mods) {
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->mWorld->onMouseButton(window, button, action, mods);
}

void Application::keyBoardCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    }
}




void Application::update(GLFWwindow *window) {
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->mWorld->render(window);
}
