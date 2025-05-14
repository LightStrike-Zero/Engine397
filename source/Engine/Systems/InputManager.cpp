#include "InputManager.h"
#include "EventSystem.h"



InputManager::InputManager(GLFWwindow* window)
    : m_window(window) {
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
        m_previousKeyStates[key] = false;

    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    m_inputState.lastMouse = { x, y };
}



void InputManager::update() {
    KeyHeld(GLFW_KEY_W);
    KeyHeld(GLFW_KEY_A);
    KeyHeld(GLFW_KEY_S);
    KeyHeld(GLFW_KEY_D);

    KeyPressed(GLFW_KEY_K);
    KeyPressed(GLFW_KEY_M);
    KeyPressed(GLFW_KEY_ESCAPE);
    KeyPressed(GLFW_KEY_X);

    // Mouse movement
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    glm::vec2 current = { x, y };
    m_inputState.mouseDelta = current - m_inputState.lastMouse;
    m_inputState.lastMouse = current;
}

// Poll a specific key and dispatch an event if its state changes from not pressed to pressed
void InputManager::KeyPressed(int key) {
    bool isDown = glfwGetKey(m_window, key) == GLFW_PRESS;
    bool wasDown = m_previousKeyStates[key];

    if (isDown && !wasDown) {
        EventSystem::getInstance().dispatchEvent(KeyPressedEvent(key));
    }

    m_previousKeyStates[key] = isDown;
}

void InputManager::KeyHeld(int key) {
    bool isDown = glfwGetKey(m_window, key) == GLFW_PRESS;
    m_inputState.keysDown[key] = isDown;
}

bool InputManager::isKeyDown(int key) const {
    auto it = m_inputState.keysDown.find(key);
    return it != m_inputState.keysDown.end() && it->second;
}
