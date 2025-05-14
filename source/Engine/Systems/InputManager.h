//
// Created by Hugo on 4/05/2025.
//

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <unordered_map>
#include <GLFW/glfw3.h>
#include "ResourceManagement/InputState.h"

class InputManager {
public:
    explicit InputManager(GLFWwindow* window);

    // void initialize();
    void update();
    const InputState& getState() { return m_inputState; }
    [[nodiscard]] bool isKeyDown(int key) const;


private:
    void KeyPressed(int key); //When register keys pressed once
    void KeyHeld(int key); //When register keys pressed continuously

    //TODO: use IWindow interface instead of GLFWwindow
    GLFWwindow* m_window;
    InputState m_inputState;

    std::unordered_map<int, bool> m_previousKeyStates;

};



#endif // INPUTSYSTEM_H
