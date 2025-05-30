#include "editor.h"
#include "panels/bar.h"
#include "imgui.h"

Editor g_Editor;

void Editor::Init() {
    // If you manually want to draw a window (hard coded) you put it here.
}

void Editor::AddPanel(const std::string& name, PanelFunction func) {
    static int uniqueId = 0;
    std::string uniqueName = name + "##" + std::to_string(uniqueId++);
    panels.emplace_back(uniqueName, func);
}

void Editor::Draw() {
    // Manually draws the bar at the top of the screen
    DrawBar();

    // Draws each panel in its own ImGui window
    for (auto it = panels.begin(); it != panels.end();) {
        bool open = true;
        if (ImGui::Begin(it->first.c_str(), &open)) {
            it->second(); // draw content only
        }
        ImGui::End();

        if (!open) {
            it = panels.erase(it);
        } else {
            ++it;
        }
    }
}
