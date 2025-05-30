#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera 
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    
    glm::mat4 getViewMatrix() const;
    void setPosition(glm::vec3 newPosition);
    glm::vec3 getPosition() const;
    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float movementSpeed = 2.5f;
    float mouseSensitivity = 0.1f;    

    float yaw;
    float pitch;

    void updateCameraVectors();
};

#endif // CAMERA_H
