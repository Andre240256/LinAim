#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stateDefinitions.hpp"

class StateStartMenu{
public:
    StateStartMenu(GLFWwindow * window);

    stateApp run();
private:
    GLFWwindow * window;

    ImVec2 buttonNormalSize;

    stateApp processInput();
};