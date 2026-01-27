#include "core/configUI.hpp"

SettingData configUI::data;
ImFont* configUI::MainFont = nullptr;
const std::string configPath = "assets/data/config.json";

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

void configUI::saveSettings()
{
    fs::path path(configPath);
    fs::path dir = path.parent_path();

    if(!fs::exists(dir)){
        try{
            fs::create_directories(dir);
        }
        catch (const fs::filesystem_error& e){
            std::cerr << "Erro, nao foi possivel criar a pasta: " << 
            e.what() << std::endl;
            return;
        }
    }

    json j = data;
    std::ofstream file(configPath);

    if(file.is_open()){
        file << j.dump(4);
        file.close();
    }
    else {
        std::cerr << "Erro, falha ao abrir arquivo para a escrita." << std::endl;
    }
}

void configUI::loadSettings()
{
    if(!fs::exists(configPath)){
        saveSettings();
    }

    std::ifstream file(configPath);

    if(file.is_open()){
        try{
            json j;
            file >> j;
            data = j.get<SettingData>();
        }
        catch (const json::parse_error& e){
            std::cerr << "Erro Json: arquivo corrompido: " << e.what() << std::endl;
        }
    }
}