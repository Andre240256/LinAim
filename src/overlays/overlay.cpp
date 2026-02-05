#include "overlays/overlay.hpp"
#include "core/game.hpp"

Overlay::Overlay(Game * ptrMaster) : game(ptrMaster) {}

bool Overlay::beginGamewindow(const std::string& title)
{
   const ImGuiViewport * viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    if(this->game->getFont() != nullptr) ImGui::PushFont(this->game->getFont());

    return ImGui::Begin(title.c_str(), nullptr, this->getOverlayFlags());
}

void Overlay::endGameWindow()
{
    ImGui::End();
    ImGui::PopFont();
}