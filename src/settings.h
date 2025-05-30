#ifndef SETTINGS_H
#define SETTINGS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Settings {
public:
    // Window properties
    inline static int windowWidth = 1280;
    inline static int windowHeight = 832;
    inline static const char* windowTitle = "LearnOpenGl";
    inline static bool fullscreen = true;

    // Display
    inline static bool vSync = true;
    inline static float fov = 60.0f;

    // Rendering
    inline static glm::vec3 windowColor = glm::vec3(0.0f, 0.0f, 0.0f);
    inline static bool msaa = true;
    inline static int msaaSamples = 8;  // 2x, 4x, or 8x
    inline static bool gama = true;
    inline static float gameValue = 2.2f;
    inline static bool wireFrame = false;
    //-----------------------------------//
    inline static bool directionalLight = false;

    // Info
    inline static float fps;
    inline static int vertices;
    inline static float cameraPosX;
    inline static float cameraPosY;
    inline static float cameraPosZ;

    // Camera settings
    inline static float nearClip = 0.01f;
    inline static float farClip = 100.0f;
    inline static float mouseSensitivity = 0.1f;
    inline static float baseMovementSpeed = 2.5f;
    inline static float sprintAcceleration = 2.25f;
    inline static float sprintMaxMultiplier = 7.5f;
};

#endif // SETTINGS_H
