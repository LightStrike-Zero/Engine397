//
// Created by Hugo on 5/05/2025.
//

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include <glm/glm.hpp>
#include <unordered_map>

struct InputState {

    glm::vec2 mouseDelta = { 0.0f, 0.0f };
    glm::vec2 lastMouse = { 0.0f, 0.0f };

    std::unordered_map<int, bool> keysDown;

    void reset() {
        mouseDelta = { 0.0f, 0.0f };
    }
};

#endif //INPUTSTATE_H
