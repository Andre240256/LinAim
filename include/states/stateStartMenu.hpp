#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/configUI.hpp"
#include "core/game.hpp"
#include "states/state.hpp"

class StateStartMenu : public State{
public:
    StateStartMenu(Game * game);
    ~StateStartMenu() noexcept override;

    void init() override;
    void processInput() override;
    void update(float dt) override;
    void render() override;
    stateApp getType() override;

    void mouseCallback(GLFWwindow * window, double x, double y) override;
    void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override;
    void keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods) override;
    void charCallback(GLFWwindow * widnow, unsigned int codepoint) override;

private:
    ImVec2 buttonNormalSize;
    ImVec2 buttonBigSize;

    void drawButtons();

};