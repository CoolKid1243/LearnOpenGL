#include "camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(2.5f), mouseSensitivity(0.1f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const 
{
    return glm::lookAt(position, position + front, up);
}

void Camera::setPosition(glm::vec3 newPosition) 
{
    position = newPosition;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        position += front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        position -= front * velocity;
    if (direction == Camera_Movement::LEFT)
        position -= right * velocity;
    if (direction == Camera_Movement::RIGHT)
        position += right * velocity;
    if (direction == Camera_Movement::UP)
        position += up * velocity;
    if (direction == Camera_Movement::DOWN)
        position -= up * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() 
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
