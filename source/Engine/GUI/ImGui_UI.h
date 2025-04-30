/**
 * @file ImGui_UI.h
 * @brief Defines the ImGuiUI class which manages rendering UI using ImGui.
 * @author Shaun and Buko
 * @date March 2025
 */

#ifndef IMGUI_UI_H
#define IMGUI_UI_H

#include <string>
#include <map>
#include "Interfaces/IGui.h"
#include "Interfaces/IWindow.h"

/**
 * @class ImGuiUI
 * @brief ImGui-based implementation of the IGui interface.
 *
 * Handles rendering of ImGui UI elements such as images, help manuals, and other GUI frames.
 */
class ImGuiUI : public IGui {
public:
    /**
     * @brief Initialise the ImGui context for a specific GLFW window.
     * @param window The GLFW window to bind ImGui to.
     */
    virtual void Initialise(GLFWwindow* window) override;

    /**
     * @brief Begin a new ImGui frame.
     */
    virtual void BeginFrame() override;

    /**
     * @brief Display an image within a dockable ImGui window.
     * @param title The name/title of the ImGui window.
     * @param imagePtr OpenGL texture ID.
     * @param imageSize Dimensions of the image.
     */
    virtual void DisplayImage(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) override;

    /**
     * @brief Displays a flipped image that is also clickable.
     * @param title The name/title of the ImGui window.
     * @param imagePtr OpenGL texture ID.
     * @param imageSize Dimensions of the image.
     * @return True if the image was clicked.
     */
    virtual bool DisplayClickableImageFlipped(const char* title, unsigned int imagePtr, const glm::vec2& imageSize) override; //buko

    /**
     * @brief End the current ImGui frame.
     */
    virtual void EndFrame() override;

    /**
     * @brief Shutdown and clean up ImGui resources.
     */
    virtual void Shutdown() override;

    /**
     * @brief Load an image and associate it with a name for reuse.
     * @param name A unique string identifier for the image.
     * @param path Path to the image file.
     */
    void loadNamedImage(const std::string& name, const std::string& path);          // buko

    /**
     * @brief Show an image from the named image registry and return click status.
     * @param name Identifier for the previously loaded image.
     * @param size Size at which to render the image.
     * @return True if the image was clicked.
     */
    bool showNamedClickableImage(const std::string& name, const glm::vec2& size);   // buko

    /**
     * @brief Show a help manual window displaying multiline text.
     * @param show Reference to a bool flag controlling visibility.
     * @param helpText The text content to be displayed.
     */
    void ShowHelpManual(bool& show, const std::string& helpText);                   // buko

    void BeginWindow(const char* name, bool* p_open, ImGuiWindowFlags flags) override;
    void EndWindow() override;

private:
    std::map<std::string, unsigned int> m_namedImages; // buko
};
#endif //IMGUI_UI_H
