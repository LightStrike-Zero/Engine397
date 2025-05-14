//
// Created by Hugo on 4/05/2025.


#ifndef GAMESTATE_H
#define GAMESTATE_H

struct GameState {
    bool shouldExit = false;
    bool wireframe = false;
    bool showHelpScreen = false;
    bool paused = false;
    // Add more runtime flags here as needed
};

#endif //GAMESTATE_H
