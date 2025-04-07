
#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "Interfaces/IWindow.h"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class GLFWWindow : public IWindow {
public:
    GLFWWindow(int width, int height, const std::string& title, bool fullscreen);
    virtual ~GLFWWindow();

    virtual bool ShouldClose() const override;
    virtual void PollEvents() override;
    virtual void SwapBuffers() override;
    virtual void GetFramebufferSize(int &width, int &height) const override;
    virtual void SetInputMode(int mode, int value) override;
    virtual float GetTime() const override { return static_cast<float>(glfwGetTime()); }
    virtual void* GetNativeWindow() const override;

private:
    GLFWwindow* m_window;
    #define CURSOR GLFW_CURSOR
    #define CURSOR_NORMAL GLFW_CURSOR_NORMAL
};

#endif //GLFWWINDOW_H
