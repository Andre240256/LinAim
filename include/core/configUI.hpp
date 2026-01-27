#pragma once

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class configUI{
public:
    static ImFont * MainFont;
    static ImGuiWindowFlags windowFlags;

    static void setup();
private:
};