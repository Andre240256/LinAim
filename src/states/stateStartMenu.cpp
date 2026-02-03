#include "states/stateStartMenu.hpp"

//constructor
//------------
StateStartMenu::StateStartMenu(Game * ptrMaster) : State(ptrMaster)
{
    this->buttonNormalSize = {200.0f, 40.0f};
    this->buttonBigSize = {400.0f, 80.0f};
    this->init();
}

StateStartMenu::~StateStartMenu() noexcept {}


void StateStartMenu::init()
{
    glfwSetInputMode(this->game->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void StateStartMenu::processInput() {}

void StateStartMenu::update (float dt) {}

void StateStartMenu::render()
{
    this->beginGameWindow("Start Menu Window");
    this->drawButtons();
    this->endGameWindow();
}

stateApp StateStartMenu::getType()
{
    return stateApp::START_MENU;
}

//callbacks functions
void StateStartMenu::mouseCallback(GLFWwindow * window, double x, double y)
{
    ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

void StateStartMenu::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void StateStartMenu::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        game->requestChangeState(stateApp::SETTINGS, stateAction::PUSH);
        return;
    }

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void StateStartMenu::charCallback(GLFWwindow * window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}

//private functions
//---------------

void StateStartMenu::drawButtons()
{
    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;

    float centerPosX = (windowWidth - this->buttonBigSize.x) * 0.5f;
    float centerPosY = windowHeight / 3.0f - this->buttonBigSize.y * 0.5f;

    ImGui::SetCursorPos(ImVec2(centerPosX, centerPosY));
    if(ImGui::Button("Play Game", this->buttonBigSize)){
        this->game->requestChangeState(stateApp::FPS, stateAction::CHANGE);
    }

    centerPosY += windowHeight / 6.0f;
    ImGui::SetCursorPos(ImVec2(centerPosX, centerPosY));
    if(ImGui::Button("Settings", this->buttonBigSize)){
        this->game->requestChangeState(stateApp::SETTINGS, stateAction::PUSH);
    }

    float yStep = windowHeight / 6.0f - this->buttonBigSize.y - this->buttonNormalSize.y * 0.5f;
    ImGui::Dummy(ImVec2(0.0f, yStep));

    centerPosX = (windowWidth - this->buttonNormalSize.x) * 0.5f;
    ImGui::SetCursorPosX(centerPosX);
    if(ImGui::Button("quit", this->buttonNormalSize)){
        this->game->requestChangeState(stateApp::EXIT, stateAction::NONE);
    }
}