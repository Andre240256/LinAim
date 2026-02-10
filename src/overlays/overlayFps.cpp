#include "overlays/overlayFps.hpp"

OverlayFps::OverlayFps(Game * ptrMaster) : Overlay(ptrMaster) {
    this->init();
    this->game->resetPlayerScore();
}

OverlayFps::~OverlayFps() {}

void OverlayFps::init() {
    this->clock = 0.0f;
    this->fpsCount = 0;
}

void OverlayFps::update(float dt)
{
    this->clock += dt;
    this->fpsCount++;

    if(this->clock >= 1.0f){
        this->fps = this->fpsCount;
        this->clock = 0.0f;
        this->fpsCount = 0.0f;
    }
}

void OverlayFps::render()
{
    this->beginGamewindow("Fps count");
    this->drawFpsStats();
    this->endGameWindow();
}

overlayType OverlayFps::getType()
{
    return overlayType::FPS;
}

void OverlayFps::drawFpsStats()
{
    std::string text = "FPS: ";

    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 windowSize = ImGui::GetWindowSize();

    float padding = windowSize.x / 12;

    float centerX = windowSize.x - (padding + textSize.x);
    float centerY = windowSize.y / 12;

    ImGui::SetCursorPos(ImVec2(centerX, centerY));
    std::string finalText = text + std::to_string(this->fps);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::Text("%s", finalText.c_str());
    ImGui::PopStyleColor();
}   