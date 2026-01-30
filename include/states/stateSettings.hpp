#pragma once

#include "core/game.hpp"

class StateSettings{
public:
    Game * game;

    float sensitivity;
    Resolution currentResolution;
    int currentResolutionIndex;

    StateSettings(Game * ptrMaster);

    void run();
    void processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

private:
    bool escPressedLastFrame;
    bool enterPressedLastFrame;

    void processInput();
};