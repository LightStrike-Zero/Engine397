//
// Created by Hugo on 4/05/2025.
//

#ifndef GAMESTATESYSTEM_H
#define GAMESTATESYSTEM_H

#include <GUI/ImGui_UI.h>

#include "EventSystem.h"
#include "ResourceManagement/GameState.h"



class GameStateSystem {
public:
    explicit GameStateSystem();

    void initialize();
    void update(ImGuiUI& Gui); // optional: may still be useful for syncing state
    GameState& getState();         // allow Lua + systems to read/write
    [[nodiscard]] const GameState& getState() const; // read-only overload

private:

    GameState m_state;
    void handleKeyPressed(const KeyPressedEvent& event);

};





#endif //GAMESTATESYSTEM_H
