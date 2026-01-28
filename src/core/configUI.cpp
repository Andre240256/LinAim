#include "core/configUI.hpp"

SettingData configUI::data;
ImFont* configUI::MainFont = nullptr;
GLFWwindow * configUI::window = nullptr;
std::vector<Resolution> configUI::availableResolutions;
int configUI::currentResolutionIndex;
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

    loadResolutions();
}

void configUI::loadResolutions()
{
    availableResolutions.clear();

    availableResolutions = {
        {1280, 960, "1280 x 960 (4:3)"},
        {1280, 1024 , "1280 x 1024 (5:4)"},
        {1680, 1000, "1680 x 1000 (16:10)"},
        {1920, 1080, "1920 x 1080 (16:9)"}
    };

    bool validResolution = false;
    for(int i = 0; i < availableResolutions.size(); i++){
        if(configUI::data.currentResolution.width == availableResolutions[i].width &&
           configUI::data.currentResolution.height == availableResolutions[i].height){
                configUI::currentResolutionIndex = i;
                validResolution = true;
            }
    }

    if(!validResolution){
        configUI::currentResolutionIndex = 3;
        configUI::data.currentResolution = availableResolutions[configUI::currentResolutionIndex];
        glfwSetWindowSize(configUI::window, configUI::data.currentResolution.width, configUI::data.currentResolution.width);
    }
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
        std::cout << "[UIConfig] Arquivo nao encontrado. Criando padrao..." << std::endl;
        saveSettings();
        return;
    }

    std::ifstream file(configPath);
    data = SettingData();

    if(file.is_open()){
        try{
            json j;
            file >> j;
            data = j.get<SettingData>();
            saveSettings();
        }
        catch (const json::parse_error& e){
            file.close();
            fs::remove(configPath);
            data = SettingData();
            saveSettings();
        }
    }
}