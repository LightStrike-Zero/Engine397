#include "GameStateSystem.h"
// #include <GL/gl.h> // or your graphics header for glPolygonMode

#include <GUI/ImGui_UI.h>

#include "Window/GLFWWindow.h"

GameStateSystem::GameStateSystem(GameState& state)
    : m_state(state), m_lastWireframe(state.wireframe), m_lastHelpScreen(state.showHelpScreen) {}

void GameStateSystem::initialize() {
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
    // if (m_state.wireframe != m_lastWireframe) {
    //     if (m_state.wireframe){}
    //     else{}
    //     m_lastWireframe = m_state.wireframe;
    // }

    // if (m_state.showHelpScreen != m_lastHelpScreen) {
    //     if (m_state.showHelpScreen) {
    //         Gui.ShowHelpManual(m_state.showHelpScreen, "helpText");//ShowHelpManual actually checks the flag lol
    //     }
    //     else {
    //         Gui.ShowHelpManual(m_state.showHelpScreen, "helpText");
    //     }
    //     m_lastHelpScreen = m_state.showHelpScreen;
    // }
    Gui.ShowHelpManual(m_state.showHelpScreen, "helpText");


}

