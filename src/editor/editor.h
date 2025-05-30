#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <functional>
#include <string>

class Editor {
public:
    using PanelFunction = std::function<void()>;

    void Init();
    void Draw();
    void AddPanel(const std::string& name, PanelFunction func);

private:
    std::vector<std::pair<std::string, PanelFunction>> panels;
};

extern Editor g_Editor; // "g" stands for global

#endif // EDITOR_H
