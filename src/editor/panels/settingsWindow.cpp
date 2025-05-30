#include "settingsWindow.h"
#include "../../settings.h"
#include "imgui.h"
#include <GL/glew.h>

void DrawSettingsWindow() {
    if (ImGui::BeginTabBar("Settings")) {
        if (ImGui::BeginTabItem("Display")) {
            ImGui::SliderInt("Window Width", &Settings::windowWidth, 640, 3840);
            ImGui::SliderInt("Window Height", &Settings::windowHeight, 480, 2160);
            ImGui::SeparatorText("");
            ImGui::Checkbox("Fullscreen", &Settings::fullscreen);
            ImGui::Checkbox("VSync", &Settings::vSync);
            ImGui::Checkbox("Wireframe", &Settings::wireFrame);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Rendering")) {
            ImGui::Checkbox("MSAA", &Settings::msaa);
            if (Settings::msaa) {
                static const int samples[3] = {2, 4, 8};
                static int sel = 1;
                for (int i = 0; i < 3; ++i)
                    if (Settings::msaaSamples == samples[i]) sel = i;
                if (ImGui::SliderInt("MSAA Samples", &sel, 0, 2, samples[sel] == 2 ? "2x" : samples[sel] == 4 ? "4x" : "8x"))
                    Settings::msaaSamples = samples[sel];
            }
            ImGui::Checkbox("Gamma", &Settings::gama);
            if (Settings::gama) {
                ImGui::SliderFloat("Gamma Value", &Settings::gameValue, 0.0f, 5.0f, "%.1f");
            }
            ImGui::SeparatorText("");
            ImGui::Checkbox("Directional Light", &Settings::directionalLight);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera")) {
            ImGui::SliderFloat("FOV", &Settings::fov, 15.0f, 165.0f, "%.1f°");
            ImGui::SliderFloat("Near Clip", &Settings::nearClip, 0.001f, 1.0f, "%.4f");
            ImGui::SliderFloat("Far Clip", &Settings::farClip, 10.0f, 1000.0f, "%.0f");
            ImGui::SeparatorText("");
            ImGui::SliderFloat("Mouse Sensitivity", &Settings::mouseSensitivity, 0.01f, 1.0f);
            ImGui::SliderFloat("Base Speed", &Settings::baseMovementSpeed, 0.1f, 10.0f);
            ImGui::SliderFloat("Sprint Accel", &Settings::sprintAcceleration, 0.0f, 10.0f);
            ImGui::SliderFloat("Sprint Max", &Settings::sprintMaxMultiplier, 1.0f, 20.0f);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    glPolygonMode(GL_FRONT_AND_BACK, Settings::wireFrame ? GL_LINE : GL_FILL);
}
