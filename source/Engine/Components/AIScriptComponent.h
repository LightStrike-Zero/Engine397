//
// Created by Hugo on 15/05/2025.
//

#ifndef AISCRIPTCOMPONENT_H
#define AISCRIPTCOMPONENT_H
#include <string>

struct AIScriptComponent {
    enum State {patrol, attack, flee};
    State currentState = patrol;
};

#endif //AISCRIPTCOMPONENT_H
