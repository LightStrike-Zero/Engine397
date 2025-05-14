#include "GameStateSystem.h"
// #include <GL/gl.h> // or your graphics header for glPolygonMode

#include <GUI/ImGui_UI.h>

#include "Window/GLFWWindow.h"


GameStateSystem::GameStateSystem(): m_state() {}


void GameStateSystem::initialize() {
    // Register key event listeners
    EventSystem::getInstance().addListener(EventType::KeyPressed,
        [this](const Event& e) {
            handleKeyPressed(static_cast<const KeyPressedEvent&>(e));
        }
    );

}

void GameStateSystem::handleKeyPressed(const KeyPressedEvent& event) {
    switch (event.keyCode) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_X:
            m_state.shouldExit = true;
        break;

        case GLFW_KEY_K:
            m_state.wireframe = !m_state.wireframe;
        break;

        case GLFW_KEY_M:
            m_state.showHelpScreen = !m_state.showHelpScreen;
        break;

        default:
            break;
    }
}

void GameStateSystem::update(ImGuiUI& Gui) {
    Gui.ShowHelpManual(m_state.showHelpScreen, "helpText");
}

