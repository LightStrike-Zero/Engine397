
#ifndef IGUI_H
#define IGUI_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class IGui
{
public:
    virtual ~IGui() = default;

    virtual void Initialise(GLFWwindow* window) = 0;

    virtual void BeginFrame() = 0;
    
    virtual void EndFrame() = 0;

    virtual void Shutdown() = 0;

    virtual void DisplayImage(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) = 0;
    
};

#endif //IGUI_H
