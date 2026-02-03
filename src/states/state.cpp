#include "states/state.hpp"

#include "core/game.hpp"

State::State(Game * ptrGame) : game(ptrGame) {};

bool State::beginGameWindow(const std::string& title)
{
    const ImGuiViewport * viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    if(this->game->getFont() != nullptr) ImGui::PushFont(this->game->getFont());

    return ImGui::Begin(title.c_str(), nullptr, game->getWindowFlagsDefault());
}

void State::endGameWindow()
{
    ImGui::End();
    ImGui::PopFont();
}