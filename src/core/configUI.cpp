#include "core/configUI.hpp"

ImFont* configUI::MainFont = nullptr;

ImGuiWindowFlags configUI::windowFlags = (
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus
    );

void configUI::setup()
{
    ImGuiIO& io = ImGui::GetIO();
    
    MainFont = io.Fonts->AddFontFromFileTTF("assets/fonts/Journal74.ttf", 28.0f);
    if(MainFont == nullptr){
        std::cout << "Aviso: Fonte personalizada não encontrada. Usando padrão." << std::endl;
        io.Fonts->AddFontDefault();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 12.0f;
    style.GrabRounding = 12.0f;
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 1.0f, 0.5f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    ImGui::StyleColorsDark();
}