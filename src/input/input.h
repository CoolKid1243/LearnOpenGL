#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera;

namespace Input {
    void init(GLFWwindow* window);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
    bool isWireframeMode();
    bool isCameraControlActive();
    void updateModeToggle(GLFWwindow* window);
}

#endif // INPUT_H
