#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/configUI.hpp"
#include "core/game.hpp"
#include "stateDefinitions.hpp"

class StateStartMenu{
public:
    Game * game;

    StateStartMenu(Game * game);
    void run();
    void processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

private:
    bool escPressedLastFrame;

    ImVec2 buttonNormalSize;
    ImVec2 buttonBigSize;

    void processInput();
    void drawButtons();
};