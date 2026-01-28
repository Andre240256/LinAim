#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

struct Resolution {
    int width = 1920;
    int height = 1080;
    std::string label = "1920 x 1080";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Resolution, width, height, label)

struct SettingData {
    float sensitivity = 1.0f;
    bool strechedRes = false;
    Resolution currentResolution;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(SettingData, sensitivity, strechedRes, currentResolution);

class configUI{
public:
    static SettingData data;
    static ImFont * MainFont;
    static ImGuiWindowFlags windowFlags;
    static std::vector<Resolution> availableResolutions;
    static int currentResolutionIndex;
    static GLFWwindow * window;

    static void setup();
    static void saveSettings();
    static void loadResolutions();
    static void loadSettings();
private:
};