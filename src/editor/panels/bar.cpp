#include "bar.h"
#include "settingsWindow.h"
#include "../editor.h"
#include "imgui.h"

void DrawBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Editor")) {
            if (ImGui::BeginMenu("Create Window")) {
                if (ImGui::MenuItem("Settings Window")) {
                    g_Editor.AddPanel("Settings Window", DrawSettingsWindow);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings")) {
            ImGui::MenuItem("Graphics");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Preferences");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Load Demo")) {
            ImGui::MenuItem("Lighting Test");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
