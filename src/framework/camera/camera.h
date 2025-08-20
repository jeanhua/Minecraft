//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include "../../core.h"
#include "../shader/shader.h"

class Camera {
public:
    Camera(float aspect,const std::string& projectionMatrixTarget,const std::string& viewMatrixTarget,float scale);
    ~Camera()=default;

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getFront() const;

    void setAspectRatio(float aspect);

public:
    // callback
    void onUpdate(GLFWwindow* window,Shader& shader);

private:
    glm::vec3 mPosition{};
    glm::vec3 mDirectionBack{};

    float aspectRatio=4.0/3.0;

    float mScale = 1.0f;
    float mSpeed=1.0f;
    float mYaw = 90.0f;
    float mPitch = 90.0f;
    float mSensitivity = 0.05f;
    float mLastX = 0.0f, mLastY = 0.0f;

    bool onMouseMove = true;

    std::string projectionMatrixTarget;
    std::string viewMatrixTarget;
};

#endif //MINECRAFT_CAMERA_H