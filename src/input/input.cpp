#include "../settings.h"
#include <GLFW/glfw3.h>
#include "input.h"
#include "../camera/camera.h"

#include <imgui.h>
#include "../extern/imgui_impl_glfw.h"
#include "../extern/imgui_impl_opengl3.h"

namespace {
    glm::vec2 lastMousePos(600, 450);
    bool firstMouse = true;

    bool wireframeKeyPressed = false;
    bool gamaKeyPressed = false;
    bool uiKeyPressed = false;
    bool uiResetKeyPressed = false;
    bool dirKeyPressed = false;

    float sprintTimer = 0.0f;
    const float sprintAccelerationRate = Settings::sprintAcceleration;
    const float maxSprintMultiplier = Settings::sprintMaxMultiplier;

    bool cameraControlActive = false;
}

void Input::init(GLFWwindow* window)
{
    glfwSetCursorPosCallback(window, Input::mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Start in UI mode
}

void Input::updateModeToggle(GLFWwindow* window)
{
    // Toggle camera control with right-click
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cameraControlActive) {
        cameraControlActive = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && cameraControlActive) {
        cameraControlActive = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Input::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cameraControlActive) return;

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!camera) return;

    if (firstMouse) {
        lastMousePos = glm::vec2(xpos, ypos);
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastMousePos.x;
    float yoffset = lastMousePos.y - ypos;
    lastMousePos = glm::vec2(xpos, ypos);

    camera->processMouseMovement(xoffset, yoffset);
}

void Input::processInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    updateModeToggle(window);

    // Toggle wireframe mode with "1"
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS && !wireframeKeyPressed) {
        Settings::wireFrame = !Settings::wireFrame;
        glPolygonMode(GL_FRONT_AND_BACK, Settings::wireFrame ? GL_LINE : GL_FILL);
        wireframeKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
        wireframeKeyPressed = false;
    }

    // Toggle gama with "2"
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS && !gamaKeyPressed) {
        Settings::gama = !Settings::gama;
        Settings::gama ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);
        gamaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) {
        gamaKeyPressed = false;
    }

    // Toggle directional light with "4"
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS && !dirKeyPressed) {
        Settings::directionalLight = !Settings::directionalLight;
        dirKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) {
        dirKeyPressed = false;
    }

    // Reset / reload all ImGui windows with "R"
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) {
        // Cleanup current ImGui context
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Reinitialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        // Reset cursor to normal mode
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Re-Initalize input
        Input::init(window);
    }

    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (!cameraControlActive) return;

    // Movement and sprinting
    bool shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    bool moving =
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;

    if (shift && moving) {
        sprintTimer += deltaTime * sprintAccelerationRate;
        if (sprintTimer > maxSprintMultiplier)
            sprintTimer = maxSprintMultiplier;
    } else {
        sprintTimer = 0.0f;
    }

    float speedMultiplier = Settings::baseMovementSpeed + sprintTimer;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::FORWARD, deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::LEFT, deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::RIGHT, deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::UP, deltaTime * speedMultiplier);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::DOWN, deltaTime * speedMultiplier);
}

bool Input::isWireframeMode() {
    return Settings::wireFrame;
}

bool Input::isCameraControlActive() {
    return cameraControlActive;
}
