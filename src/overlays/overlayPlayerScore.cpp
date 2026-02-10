#include "overlays/overlayPlayerScore.hpp"
#include "core/game.hpp"
OverlayPlayerScore::OverlayPlayerScore(Game * ptrMaster) : Overlay(ptrMaster)
{
    this->init();
}

OverlayPlayerScore::~OverlayPlayerScore() {};

void OverlayPlayerScore::init()
{
    this->targetHits = 0;
    this->shoots = 0;
}

void OverlayPlayerScore::update(float dt)
{
    PlayerScore * playerScore = this->game->getPlayerScore();
    if(playerScore == nullptr){
        std::cout << "ptr nulo" << std::endl;
        exit(1);
        return;
    }
    this->targetHits = playerScore->targetHit;
    this->shoots = playerScore->shoots;
}

void OverlayPlayerScore::render()
{
    this->beginGamewindow("Score Window");
    this->drawPlayerScore();
    this->endGameWindow();
}

overlayType OverlayPlayerScore::getType()
{
    return overlayType::PLAYER_SCORE;
}

void OverlayPlayerScore::drawPlayerScore()
{
    float precision = 0.0f;
    if(this->shoots != 0)
        precision = 100 * static_cast<float>(this->targetHits) / static_cast<float>(this->shoots);
    int integer_part = (int)precision;
    float fractionary_part = precision - (float)integer_part;
    std::string text = "Targets Hit: " + std::to_string(this->targetHits) 
        + " | Shoots: " + std::to_string(this->shoots)
        + " | Acurracy: " + std::to_string(integer_part) + "." + std::to_string((int)(100 * fractionary_part)) + "%";

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    float centerX = (windowSize.x - textSize.x) * 0.5;

    ImGui::SetCursorPosX(centerX);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::Text("%s", text.c_str());
    ImGui::PopStyleColor();
}
