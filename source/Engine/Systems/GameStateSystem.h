//
// Created by Hugo on 4/05/2025.
//

#ifndef GAMESTATESYSTEM_H
#define GAMESTATESYSTEM_H

#include <GUI/ImGui_UI.h>

#include "EventSystem.h"
#include "ResourceManagement/GameState.h"


#include "ResourceManagement/GameState.h"
#include "EventSystem.h"

class GameStateSystem {
public:
    explicit GameStateSystem(GameState& state);

    void initialize();
    void update(ImGuiUI& Gui); // optional: may still be useful for syncing state

private:
    GameState& m_state;
    bool m_lastWireframe = false;
    bool m_lastHelpScreen = false;

    void handleKeyPressed(const KeyPressedEvent& event);
    static void enableWireframeMode();
    static void disableWireframeMode();
};





#endif //GAMESTATESYSTEM_H
