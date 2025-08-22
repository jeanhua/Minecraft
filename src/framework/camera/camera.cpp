//
// Created by jeanhua on 2025/8/16.
//

#include "camera.h"

#include "../../application/chunk/Chunk.h"

float getAngleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
    const float dotProduct = glm::dot(a, b);
    const float lengthA = glm::length(a);
    const float lengthB = glm::length(b);
    if (lengthA == 0.0f || lengthB == 0.0f) {
        return 0.0f;
    }
    float cosTheta = dotProduct / (lengthA * lengthB);
    cosTheta = glm::clamp(cosTheta, -1.0f, 1.0f);
    return std::acos(cosTheta);
}

Camera::Camera(float aspect, const std::string& projectionMatrixTarget,
               const std::string& viewMatrixTarget, float scale) {
    mPosition = glm::vec3(0.0f, 200.0f, 0.0f);
    mYaw = 0.0f;
    mPitch = -30.0f;
    updateCameraVectors();
    this->aspectRatio = aspect;
    this->viewMatrixTarget = viewMatrixTarget;
    this->projectionMatrixTarget = projectionMatrixTarget;
    this->mScale = scale;

    printf("Camera initial:\n");
    printf("Position: (%.2f, %.2f, %.2f)\n", mPosition.x, mPosition.y, mPosition.z);
    printf("Scale: %.3f\n", mScale);
    printf("Yaw: %.2f, Pitch: %.2f\n", mYaw, mPitch);
    printf("Front: (%.3f, %.3f, %.3f)\n", mFront.x, mFront.y, mFront.z);
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

    mFront = glm::normalize(front);
    mDirectionBack = -mFront;

    mRight = glm::normalize(glm::cross(mFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f * SOLID_SIZE * mScale);
}

void Camera::onUpdate(GLFWwindow *window, Shader &shader) {
    static double lastTime = glfwGetTime();
    const double delta = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    const float velocity = static_cast<float>(delta) * mSpeed / mScale;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mPosition += mFront * velocity;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mPosition -= mFront * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mPosition -= mRight * velocity;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mPosition += mRight * velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        mPosition.y += velocity;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        mPosition.y -= velocity;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        onMouseMove = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (onMouseMove == false) {
            onMouseMove = true;
            mLastX = static_cast<float>(x_pos);
            mLastY = static_cast<float>(y_pos);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (onMouseMove) {
        const float x_offset = static_cast<float>(x_pos - mLastX) * mSensitivity;
        const float y_offset = static_cast<float>(mLastY - y_pos) * mSensitivity;
        mLastX = static_cast<float>(x_pos);
        mLastY = static_cast<float>(y_pos);

        mYaw += x_offset;
        mPitch += y_offset;

        if (mPitch > 89.0f) mPitch = 89.0f;
        if (mPitch < -89.0f) mPitch = -89.0f;

        updateCameraVectors();
    }

    shader.setMat4(viewMatrixTarget, getViewMatrix());
    shader.setMat4(projectionMatrixTarget, getProjectionMatrix());
}

void Camera::setAspectRatio(float aspect) {
    this->aspectRatio = aspect;
}

glm::vec3 Camera::getPosition() const {
    return mPosition;
}

glm::vec3 Camera::getFront() const {
    return mFront;
}