#include "core/game.hpp"

#include "states/stateFps.hpp"
#include "states/stateSettings.hpp"
#include "states/stateStartMenu.hpp"

//constructor
Game::Game()
{
    this->loadSettings();       
    this->loadOpenGL();         
    this->loadGlad();          
    this->loadImGui(); 
    this->loadCallBacks();        
    this->loadResolutions();   
    this->setup();             
}

Game::~Game()
{
    this->shutdownImGui();

    if(this->stateFps != nullptr) delete this->stateFps;
    if(this->stateStartMenu != nullptr) delete this->stateStartMenu;
    if(this->stateSettings != nullptr) delete this->stateSettings;

    this->availableResolutions.clear();
    
    glfwTerminate();
}

void Game::run()
{
    this->currentState = stateApp::START_MENU;
    this->lastState = stateApp::START_MENU;

    this->stateFps = new StateFps(this);

    while(this->currentState != stateApp::EXIT)
        switch(this->currentState){
            case stateApp::START_MENU:
                if(this->stateStartMenu == nullptr){
                    this->stateStartMenu = new StateStartMenu(this);
                }
                this->stateStartMenu->run();
                if(this->currentState == stateApp::SETTINGS)
                    this->lastState = stateApp::START_MENU;
                break;

            case stateApp::FPS:
                if(this->stateFps == nullptr){
                    this->stateFps = new StateFps(this);
                    std::cout << "Creating new object fps state" <<std::endl;
                }
                this->stateFps->run();
                if(this->currentState == stateApp::SETTINGS){
                    this->lastState = stateApp::FPS;
                };
                break;

            case stateApp::SETTINGS:
                if(this->stateSettings == nullptr){
                    this->stateSettings = new StateSettings(this);
                }
                this->stateSettings->run();
                break;
            
            case stateApp::EXIT:
                break;
        }
}

void Game::applySettingChanges(float sensitivity, int currentResolutionIndex)
{
    this->settings.currentResolution = this->availableResolutions[currentResolutionIndex];
    this->settings.sensitivity = sensitivity;
    this->currentResolutionIndex = currentResolutionIndex;

    this->saveSettings();
}

int Game::loadOpenGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif 

    int count;
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * modes = glfwGetVideoModes(monitor, &count);

    int bestIndex = 0;
    for(int i = 0; i < count; i++){
        if(modes[i].refreshRate > modes[bestIndex].refreshRate){
            bestIndex = i;
        }
        else if(modes[i].refreshRate == modes[bestIndex].refreshRate){
            if(modes[i].width > modes[bestIndex].width)
                bestIndex = i;
        }
    }

    glfwWindowHint(GLFW_RED_BITS, modes[bestIndex].redBits);
    glfwWindowHint(GLFW_GREEN_BITS, modes[bestIndex].greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, modes[bestIndex].blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, modes[bestIndex].refreshRate);

    int width = modes[bestIndex].width;
    int height = modes[bestIndex].height;

    this->window = glfwCreateWindow(width, height, "LinAim", monitor, NULL);
    if(!this->window){
        std::cout << "[Game::loadOpenGL] Failed to create window" << std::endl;
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    return 1;
}

int Game::loadGlad()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "[Game::loadGlad] Failed to initialize glad" << std::endl;
        return 0;
    }

    int width, height;
    glfwGetWindowSize(this->window, &width, &height);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    return 1;
}

int Game::loadImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 1;
}

int Game::shutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 1;
}

int Game::loadSettings()
{
    if(!fs::exists(this->settingsPath)){
        std::cout << "[Game::loadSettings] File not found. Creating the default." << std::endl;
        this->saveSettings();
        return 0;
    }

    std::ifstream file(this->settingsPath);
    this->settings = SettingData();

    if(file.is_open()){
        try{
            json j;
            file >> j;
            this->settings = j.get<SettingData>();
            this->saveSettings();
        }
        catch (const json::parse_error& e){
            std::cout << "[Game::loadSettings] JSON not reachable, file probabily corrupted." << std::endl;
            file.close();
            fs::remove(this->settingsPath);
            this->settings = SettingData();
            this->saveSettings();
            return 0;
        }
    }

    return 1;
}

int Game::loadCallBacks()
{
    glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_size_callback_wrapper);
    glfwSetCursorPosCallback(this->window, Game::mouse_callback_wrapper);
    glfwSetMouseButtonCallback(this->window, Game::mouse_button_callback_wrapper);
    glfwSetKeyCallback(this->window, Game::key_callback_wrapper);
    glfwSetCharCallback(this->window, Game::char_callback_wrapper);

    return 1;
}

int Game::saveSettings()
{
    fs::path path(this->settingsPath);
    fs::path dir = path.parent_path();

    if(!fs::exists(dir)){
        std::cout << "[Game::saveSettings] Director: " << dir << " not found." <<  std::endl;
        try{
            fs::create_directories(dir);
        }
        catch (const fs::filesystem_error& e){
            std::cout << "[Game::saveSettings] Error creating directory: " << dir << std::endl;
            std::cout << "\"\"\"" << e.what() << "\"\"\"" << std::endl;
            return 0;
        }
    }

    json j = this->settings;
    std::ofstream file(this->settingsPath);

    if(file.is_open()){
        file << j.dump(4);
        file.close();
    }
    else {
        std::cout << "[Game::saveSettings] Error opening file: " << this->settingsPath << std::endl;
        return 0;
    }

    return 1;
}

int Game::loadResolutions()
{
    this->availableResolutions.clear();

    this->availableResolutions = {
        {1280, 960, "1280 x 960 (4:3)"},
        {1280, 1024 , "1280 x 1024 (5:4)"},
        {1680, 1000, "1680 x 1000 (16:10)"},
        {1920, 1080, "1920 x 1080 (16:9)"}
    };

    bool validResolution = false;
    bool eqHeight, eqWidth;
    for(int i = 0; i < this->availableResolutions.size() && !validResolution; i++){
        eqHeight = this->settings.currentResolution.height == this->availableResolutions[i].height;
        eqWidth = this->settings.currentResolution.width == this->availableResolutions[i].width;
        if(eqHeight && eqWidth){
            this->currentResolutionIndex = i;
            validResolution = true;
        }
    }

    if(!validResolution){
        std::cout << "[Game::loadResolutions] Not a valid Resolution from json" << std::endl;
        this->currentResolutionIndex = 3;
        this->settings.currentResolution = this->availableResolutions[this->currentResolutionIndex];
        glfwSetWindowSize(this->window, this->settings.currentResolution.width, this->settings.currentResolution.height);
        return 0;
    }

    return 1;
}

int Game::setup()
{
    ImGuiIO& io = ImGui::GetIO();

    this->configUI.mainFont = io.Fonts->AddFontFromFileTTF("assets/fonts/Journal74.ttf", 28.0f);
    if(configUI.mainFont == nullptr){
        std::cout << "[Game::setup] Error loading unique font from: " << "assets/fonts/Journal74.ttf" << std::endl;
        std::cout << "Loading default font" << std::endl;
        io.Fonts->AddFontDefault(); 

        return 0;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 12.0f;
    style.GrabRounding = 12.0f;
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 1.0f, 0.5f);
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImGui::StyleColorsDark();

    return 1;
}


//callbacks wrappers
//---------------
void Game::framebuffer_size_callback_wrapper(GLFWwindow * window, int width ,int height)
{
    Game * game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    if(game == nullptr){
        return;
    }

    game->framebuffer_size_callback(window, width, height);
}

void Game::mouse_callback_wrapper(GLFWwindow * window, double xpos, double ypos)
{
    Game * game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    if(game == nullptr){
        return;
    }

    game->mouse_callback(window, xpos, ypos);
}

void Game::mouse_button_callback_wrapper(GLFWwindow * window, int button, int actions, int mods)
{
     Game * game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    if(game == nullptr){
        return;
    }

    game->mouse_button_callback(window, button, actions, mods);
}

void Game::key_callback_wrapper(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    Game * game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    if(game == nullptr){
        return;
    }

    game->key_callback(window, key, scancode, action, mods);
}

void Game::char_callback_wrapper(GLFWwindow * window, unsigned int codepoint)
{
    Game * game = static_cast<Game *>(glfwGetWindowUserPointer(window));

    if(game == nullptr){
        return;
    }

    game->char_callback(window, codepoint);
}

//callbacks
//-------------
void Game::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
{
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
        ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
        return;
    }
    else if(this->stateFps != nullptr){
        this->stateFps->player.updateDir(window, xposIn, yposIn);
    }
}

void Game::mouse_button_callback(GLFWwindow * window, int button, int actions, int mods)
{
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
        ImGui_ImplGlfw_MouseButtonCallback(window, button, actions, mods);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return;
        }
    }

    if(button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS && stateFps != nullptr){
        this->stateFps->shoot();
    }
}

void Game::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{

    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }

    switch(this->currentState){
        case stateApp::FPS:
            if(this->stateFps != nullptr) this->stateFps->processKeyCallback(window, key, scancode, action, mods);
            break;
        case stateApp::SETTINGS:
            if(this->stateSettings != nullptr) this->stateSettings->processKeyCallback(window, key, scancode, action, mods);
            break;
        case stateApp::START_MENU:
            if(this->stateStartMenu != nullptr) this->stateStartMenu->processKeyCallback(window, key, scancode, action, mods);
            break;
    }
}

void Game::char_callback(GLFWwindow * window, unsigned int codepoint)
{
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
        ImGui_ImplGlfw_CharCallback(window, codepoint);
    }
}