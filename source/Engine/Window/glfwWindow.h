/**
 * @file glfwWindow.h
 * @brief GLFW-based implementation of the IWindow interface for handling windowing and input events.
 * @author Shaun + a function by Buko
 * @date March 2025
 */

#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H

#include "Interfaces/IWindow.h"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @class GLFWWindow
 * @brief Manages a window using the GLFW library and implements the IWindow interface.
 */
class GLFWWindow : public IWindow {

public:

    /**
     * @brief Constructs a new GLFWWindow.
     * @param width Width of the window in pixels.
     * @param height Height of the window in pixels.
     * @param title Title of the window.
     * @param fullscreen Whether to start the window in fullscreen mode.
     */
    GLFWWindow(int width, int height, const std::string& title, bool fullscreen);

    /**
     * @brief Destroys the GLFWWindow and releases resources.
     */
    virtual ~GLFWWindow();

    /**
     * @brief Destroys the GLFWWindow and releases resources.
     */
    virtual bool ShouldClose() const override;

    /**
     * @brief Polls input events from the operating system.
     */
    virtual void PollEvents() override;

    /**
     * @brief Swaps the front and back buffers.
     */
    virtual void SwapBuffers() override;

    /**
     * @brief Gets the current size of the framebuffer.
     * @param width Output parameter for framebuffer width.
     * @param height Output parameter for framebuffer height.
     */
    virtual void GetFramebufferSize(int &width, int &height) const override;

    /**
     * @brief Sets an input mode, such as cursor visibility.
     * @param mode The input mode to set (e.g., GLFW_CURSOR).
     * @param value The value for the input mode (e.g., GLFW_CURSOR_DISABLED).
     */
    virtual void SetInputMode(int mode, int value) override;

    /**
     * @brief Gets the current time since GLFW was initialized.
     * @return Elapsed time in seconds.
     */
    virtual float GetTime() const override { return static_cast<float>(glfwGetTime()); }

    /**
     * @brief Gets the native window handle (GLFWwindow pointer).
     * @return A pointer to the native GLFWwindow.
     */
    virtual void* GetNativeWindow() const override;



    /**
     * @brief Sets whether the window should close.
     * @param value True to request window close, false to keep open.
     */
    virtual void SetShouldClose(bool value) override;  //buko

private:

    GLFWwindow* m_window;               ///< The GLFW window handle
    #define CURSOR GLFW_CURSOR
    #define CURSOR_NORMAL GLFW_CURSOR_NORMAL
};

#endif //GLFWWINDOW_H
