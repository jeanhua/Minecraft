//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_APPLICATION_H
#define MINECRAFT_APPLICATION_H

#include "../core.h"
#include "world/world.h"

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

    world* mWorld = nullptr;

    uint16_t mFPS = 120;
private:
    static void frameBufferCallBack(GLFWwindow* window, int width, int height);
    static void keyBoardCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallBack(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallBack(GLFWwindow* window, double xpos, double ypos);

private:
    static void update(GLFWwindow* window);
};

#endif //MINECRAFT_APPLICATION_H