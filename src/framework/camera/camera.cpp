//
// Created by jeanhua on 2025/8/16.
//

#include "camera.h"

#include "../../application/chunk/Chunk.h"

Camera::Camera(float aspect,const std::string& projectionMatrixTarget,const std::string& viewMatrixTarget,float scale) {
    mPosition = glm::vec3(0.0f, 100.0f, 20.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    mDirectionBack = glm::normalize(mPosition - target);
    this->aspectRatio = aspect;
    this->viewMatrixTarget = viewMatrixTarget;
    this->projectionMatrixTarget = projectionMatrixTarget;
    this->mScale = scale;
}


glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mDirectionBack));
    glm::vec3 cameraUp = glm::normalize(glm::cross(mDirectionBack, right));
    return glm::lookAt(mPosition, mPosition - mDirectionBack, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f * SOLID_SIZE * mScale);
}

void Camera::onUpdate(GLFWwindow *window, Shader &shader) {
    static double lastTime = glfwGetTime();
    double delta = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    // move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mPosition += (-mDirectionBack)/mScale * static_cast<float>(delta) * mSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mPosition -= (-mDirectionBack)/mScale * static_cast<float>(delta) * mSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mDirectionBack);
        mPosition -= right/mScale * static_cast<float>(delta) * mSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mDirectionBack);
        mPosition += right/mScale * static_cast<float>(delta) * mSpeed;
    }

    if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS) {
        mPosition.y += static_cast<float>(delta * mSpeed)/mScale;
    } else if (glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        mPosition.y -= static_cast<float>(delta * mSpeed)/mScale;
    }

    // view
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);

        if (mFirstMouse) {
            mLastX = static_cast<float>(x_pos);
            mLastY = static_cast<float>(y_pos);
            mFirstMouse = false;
        }

        const float x_offset = static_cast<float>(x_pos - mLastX) * mSensitivity;
        const float y_offset = static_cast<float>(y_pos - mLastY) * mSensitivity;
        mLastX = static_cast<float>(x_pos);
        mLastY = static_cast<float>(y_pos);

        mYaw += x_offset;
        mPitch += y_offset;

        if (mPitch > 89.0f) mPitch = 89.0f;
        if (mPitch < -89.0f) mPitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        direction.y = sin(glm::radians(mPitch));
        direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        mDirectionBack = glm::normalize(direction);
    } else {
        mFirstMouse = true;
    }
    shader.setMat4(viewMatrixTarget,getViewMatrix());
    shader.setMat4(projectionMatrixTarget,getProjectionMatrix());
}

void Camera::setAspectRatio(float aspect) {
    this->aspectRatio = aspect;
}

glm::vec3 Camera::getPosition() const {
    return mPosition;
}
