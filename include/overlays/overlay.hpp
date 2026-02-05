#pragma once

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Game;
enum class overlayType;

class Overlay{
public:
    Overlay(Game * ptrMaster);
    virtual ~Overlay() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual overlayType getType() = 0;

protected:
    bool active = false;
    Game * game = nullptr;

    bool beginGamewindow(const std::string& title);
    void endGameWindow();

    ImGuiWindowFlags getOverlayFlags() const { return overlayFlags; }
    ImGuiWindowFlags getOverlayFlagsInteractive() const { return overlayFlagsInteractive; }

private:
    const ImGuiWindowFlags overlayFlags = (
        ImGuiWindowFlags_NoDecoration       | 
        ImGuiWindowFlags_NoMove             | 
        ImGuiWindowFlags_NoResize           | 
        ImGuiWindowFlags_NoSavedSettings    | 
        ImGuiWindowFlags_NoBackground       | 
        ImGuiWindowFlags_NoInputs           
    );

    const ImGuiWindowFlags overlayFlagsInteractive = (
        ImGuiWindowFlags_NoDecoration       | 
        ImGuiWindowFlags_NoMove             | 
        ImGuiWindowFlags_NoResize           | 
        ImGuiWindowFlags_NoSavedSettings    | 
        ImGuiWindowFlags_NoBackground       
    );
};