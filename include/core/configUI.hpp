#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <iomanip>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

struct SettingData {
    float sensitivity = 1.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SettingData, sensitivity);

class configUI{
public:
    static SettingData data;
    static ImFont * MainFont;
    static ImGuiWindowFlags windowFlags;

    static void setup();
    static void saveSettings();
    static void loadSettings();
private:
};