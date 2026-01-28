#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/configUI.hpp"

#include "states/stateDefinitions.hpp"

class StateSettings{
public:
    float sensitivity;
    Resolution currentResolution;
    int currentResolutionIndex;

    StateSettings(GLFWwindow * window);

    stateApp run(stateApp lastState);

private:
    GLFWwindow * window;
    bool escPressedLastFrame;
    bool enterPressedLastFrame;

    stateApp processInput(stateApp lastState);
    int applySettingsChanges();
};