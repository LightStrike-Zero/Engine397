
#ifndef IMGUI_UI_H
#define IMGUI_UI_H
#include "Interfaces/IGui.h"



class ImGuiUI : public IGui {
public:
    virtual void Initialise(GLFWwindow* window) override;
    
    virtual void BeginFrame() override;
    
    virtual void DisplayImage(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) override;
  
    virtual void EndFrame() override;
    
    virtual void Shutdown() override;
};
#endif //IMGUI_UI_H
