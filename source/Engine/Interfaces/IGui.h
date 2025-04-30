/**
 * @file IGui.h
 * @brief  Interface for ImGui-based GUI systems.
 * @author Probably Shaun + 1 function by Buko
 * @date March 2025
 */

#ifndef IGUI_H
#define IGUI_H

#include <imgui.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

/**
 * @class IGui
 * @brief Abstract interface for GUI rendering using ImGui.
 */
class IGui
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IGui() = default;

    virtual void BeginWindow(const char* name,
                             bool* p_open = nullptr,
                             ImGuiWindowFlags flags = 0) = 0;
    virtual void EndWindow() = 0;

    /**
     * @brief Initialise the GUI system with the GLFW window context.
     * @param window Pointer to the GLFW window.
     */
    virtual void Initialise(GLFWwindow* window) = 0;

    /**
     * @brief Start a new ImGui frame.
     */
    virtual void BeginFrame() = 0;

    /**
     * @brief End and render the current ImGui frame.
     */
    virtual void EndFrame() = 0;

    /**
     * @brief Clean up any ImGui resources before shutdown.
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Display a 2D image in an ImGui window.
     * @param title Title of the ImGui window.
     * @param imagePtr OpenGL texture ID.
     * @param imageSize Size of the image in pixels.
     */
    virtual void DisplayImage(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) = 0;

    /**
     * @brief Display a clickable, vertically flipped image in an ImGui window.
     * @param title Title of the ImGui window.
     * @param imagePtr OpenGL texture ID.
     * @param imageSize Size of the image in pixels.
     * @return True if the image was clicked.
     */
    virtual bool DisplayClickableImageFlipped(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) = 0; //buko
    
};

#endif //IGUI_H
