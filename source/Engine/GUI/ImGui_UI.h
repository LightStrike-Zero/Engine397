
#ifndef IMGUI_UI_H
#define IMGUI_UI_H

#include <string>
#include <map>
#include "Interfaces/IGui.h"

#include "Interfaces/IWindow.h"


class ImGuiUI : public IGui {
public:
    virtual void Initialise(GLFWwindow* window) override;
    
    virtual void BeginFrame() override;
    
    virtual void DisplayImage(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) override;

    virtual bool DisplayClickableImageFlipped(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) override; //buko
  
    virtual void EndFrame() override;
    
    virtual void Shutdown() override;

    void loadNamedImage(const std::string& name, const std::string& path); // buko
    bool showNamedClickableImage(const std::string& name, const glm::vec2& size);   // buko

private:
    std::map<std::string, unsigned int> m_namedImages; // buko
};
#endif //IMGUI_UI_H
