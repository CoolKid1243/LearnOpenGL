#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include "shader.h"
#include "settings.h"
#include "camera/camera.h"
#include "input/input.h"
#include "editor/editor.h"
#include "object/object.h"

#include <imgui.h>
#include "extern/imgui_impl_glfw.h"
#include "extern/imgui_impl_opengl3.h"

#include <iostream>

// Global timing
static float deltaTime = 0.0f, lastFrame = 0.0f;

// Forward declarations
void framebuffer_size_callback(GLFWwindow* w, int width, int height);
void processInput(GLFWwindow* w);
unsigned int loadTexture(const char* path, bool gamma);

int main() {
    // --- GLFW & OpenGL init ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, Settings::msaaSamples);
    #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    if (Settings::fullscreen) {
        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        Settings::windowWidth  = mode->width;
        Settings::windowHeight = mode->height;
    }

    GLFWwindow* window = glfwCreateWindow(
        Settings::windowWidth, Settings::windowHeight,
        Settings::windowTitle,
        Settings::fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(Settings::vSync);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // --- ImGui init ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();
    g_Editor.Init();
 
    // --- Shaders & textures ---
    Shader defaultShader("shaders/basicShader.vert","shaders/basicShader.frag");
    Shader lightingShader("shaders/basicLightingShader.vert","shaders/basicLightingShader.frag");

    unsigned int woodDiff  = loadTexture("textures/woodContainer.png",    true);
    unsigned int woodSpec  = loadTexture("textures/woodContainerSpec.png", false);
    unsigned int panelDiff = loadTexture("textures/woodPannels.png",     true);
    unsigned int panelSpec = loadTexture("textures/woodPannelsSpec.png", false);

    lightingShader.use();
    lightingShader.setInt("material.diffuse",  0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setFloat("gamma", Settings::gameValue);
    Settings::msaa ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

    // --- Camera & input ---
    Camera camera({0,0,3},{0,1,0},-90,0);
    glfwSetWindowUserPointer(window, &camera);
    Input::init(window);

    // --- Create scene ---
    Object::Create(
        Object::Type::PointLight,
        {1,1,1}, 
        {0.2f,0.2f,0.2f}, 
        {2, 2.5f, 1}, 
        0,0,2   // None of these values work
    );
    Object::Create(
        Object::Type::PointLight,
        {1,1,1}, 
        {0.2f,0.2f,0.2f}, 
        {-2, 2.5f, 4}, 
        0,0,2   // None of these values work
    );
    // Object::Create(
    //     Object::Type::Sphere,    
    //     {0.7f,0.2f,1},   
    //     {0.5f,0.5f,0.5f}, 
    //     {2,1,-4}
    // );
    Object::Create(
        Object::Type::Cube,
        {1,1,1},
        {1,1,1},
        {0,1,0},
        woodDiff,
        woodSpec
    );
    Object::Create(
        Object::Type::Plane,     
        {0.8f,0.8f,0.8f}, 
        {3,3,3}, 
        {0,0,0}, 
        panelDiff, 
        panelSpec
    );

    // --- Main loop ---
    while (!glfwWindowShouldClose(window)) {
        // Delta time
        float now = glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        processInput(window);

        // Clear frame
        glClearColor(
            Settings::windowColor.r,
            Settings::windowColor.g,
            Settings::windowColor.b,
            1.0f
        );
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Animate light
        // float lightRadius = 3.0f;
        // float lightSpeed = 1.0f;
        // float currentTime = glfwGetTime();
        // lightPos.x = lightRadius * cos(lightSpeed * currentTime);
        // lightPos.y = 2.5f + 0.4f * sin(5.0f * lightSpeed * currentTime);
        // lightPos.z = lightRadius * sin(lightSpeed * currentTime);

        // Camera matrices
        glm::mat4 proj = glm::perspective(
            glm::radians(Settings::fov),
            (float)Settings::windowWidth/Settings::windowHeight,
            Settings::nearClip, Settings::farClip
        );
        glm::mat4 view = camera.getViewMatrix();

        // Draw with lighting
        lightingShader.use();
        lightingShader.setMat4("projection", proj);
        lightingShader.setMat4("view", view);
        lightingShader.setVec3("viewPos", camera.getPosition());
        lightingShader.setFloat("material.shininess", 64);
        if (Settings::directionalLight) {
            lightingShader.setVec3("dirLight.direction", -0.2f,-1.0f,-0.3f);
            lightingShader.setVec3("dirLight.ambient",   0.05f,0.05f,0.05f);
            lightingShader.setVec3("dirLight.diffuse",   0.4f, 0.4f, 0.4f);
            lightingShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);
        } else {
            lightingShader.setVec3("dirLight.ambient",   0,0,0);
            lightingShader.setVec3("dirLight.diffuse",   0,0,0);
            lightingShader.setVec3("dirLight.specular",  0,0,0);
        }
        defaultShader.use();
        defaultShader.setMat4("projection", proj);
        defaultShader.setMat4("view", view);
        Object::RenderAll(defaultShader, lightingShader);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        g_Editor.Draw();

        // Render ImGui
        glDisable(GL_FRAMEBUFFER_SRGB);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        Settings::gama ? glEnable(GL_FRAMEBUFFER_SRGB) : glDisable(GL_FRAMEBUFFER_SRGB);    // Apply gama

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* w) {
    Camera& cam = *static_cast<Camera*>(glfwGetWindowUserPointer(w));
    Input::processInput(w, cam, deltaTime);
}

unsigned int loadTexture(const char* path, bool gamma) {
    unsigned int id;
    glGenTextures(1,&id);
    int w,h,n;
    unsigned char* data = stbi_load(path,&w,&h,&n,0);
    if (data) {
        GLenum internal = (n==3?(gamma?GL_SRGB:GL_RGB):n==4?(gamma?GL_SRGB_ALPHA:GL_RGBA):GL_RED);
        GLenum format   = (n==1?GL_RED:(n==3?GL_RGB:GL_RGBA));
        glBindTexture(GL_TEXTURE_2D,id);
        glTexImage2D(GL_TEXTURE_2D,0,internal,w,h,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        stbi_image_free(data);
    } else {
        std::cout<<"Failed to load texture: "<<path<<std::endl;
        stbi_image_free(data);
    }
    return id;
}
