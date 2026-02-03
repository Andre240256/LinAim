#include "states/stateSettings.hpp"

StateSettings::StateSettings(Game * ptrMaster) : State(ptrMaster)
{
    this->init();
}

StateSettings::~StateSettings()
{}

void StateSettings::init()
{
    settingCopy = this->game->getSettings();
    this->currentResolutionIndex = this->game->getResolutionIndex();
    glfwSetInputMode(this->game->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void StateSettings::processInput() {}

void StateSettings::update(float dt) {}

void StateSettings::render()
{
    this->beginGameWindow("Settings window");
    this->drawButtons();
    this->endGameWindow();
}

stateApp StateSettings::getType()
{
    return stateApp::SETTINGS;
}

//callback functions

void StateSettings::mouseCallback(GLFWwindow * window, double x, double y)
{
    ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

void StateSettings::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void StateSettings::keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods)
{
    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        this->game->applySettingChanges(settingCopy.sensitivity, this->currentResolutionIndex);
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        this->game->requestChangeState(stateApp::NONE, stateAction::POP);
    }
    ImGui_ImplGlfw_KeyCallback(window, key, sancode, action, mods);
}

void StateSettings::charCallback(GLFWwindow * window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}

//private functions
//---------------------
void StateSettings::drawButtons()
{
    float& sensitivity = this->settingCopy.sensitivity;
    Resolution& resolution = this->settingCopy.currentResolution;
    this->currentResolutionIndex = this->game->getResolutionIndex();

    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;
    float textSize = ImGui::CalcTextSize("Sensitivity").x;

    float boxSize = 100.0f;
    float sliderSize = windowWidth - boxSize - windowWidth /6.0f;
    float sliderCenter = windowWidth - windowWidth / 18.0f - sliderSize / 2.0f;
    float boxCenter = windowWidth / 12.0f + boxSize * 0.5f;
    
    float stepX = sliderCenter - textSize * 0.5f;
    float stepY = windowHeight / 6;

    ImGui::SetCursorPosX(stepX);
    ImGui::SetCursorPosY(stepY);
    ImGui::Text("Sensitivity");

    stepX = sliderCenter - sliderSize * 0.5f;
    ImGui::SetCursorPosX(stepX);
    ImGui::PushItemWidth(sliderSize);
    ImGui::SliderFloat("##sens_slider", &sensitivity , 0.0001f, 5.0f);
    ImGui::PopItemWidth();

    ImGui::SameLine();

    stepX = boxCenter - boxSize * 0.5f;
    ImGui::SetCursorPosX(stepX);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushItemWidth(boxSize);
    if(ImGui::InputFloat("##sens_input", &sensitivity, 0.0f, 0.0f, "%.3f")){
        if(sensitivity < 0.0001f) sensitivity = 0.0001f;
        if(sensitivity > 5.0f) sensitivity = 5.0f;
    }

    ImGui::PopItemWidth();
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(0.0f, 40.0f));

    textSize = ImGui::CalcTextSize("Resolution").x;
    stepX = sliderCenter - textSize * 0.5f;
    ImGui::SetCursorPosX(stepX);
    ImGui::Text("Resolution");

    stepX = sliderCenter - sliderSize * 0.5;
    ImGui::SetCursorPosX(stepX);
    ImGui::PushItemWidth(sliderSize);
    const char * previewValue = resolution.label.c_str();

    std::vector <Resolution>& resolutionsList = this->game->getAvailableResolutions();
    if(ImGui::BeginCombo("##Resolution_combo", previewValue)){
        for(int n = 0; n < resolutionsList.size(); n++){
            const bool isSelected = (this->currentResolutionIndex == n);

            if(ImGui::Selectable(resolutionsList[n].label.c_str(), isSelected)){
                this->currentResolutionIndex = n;
                resolution = resolutionsList[n];
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();

    ImVec2 buttonNormalSize = {200.0f, 40.0f};
    ImGui::SetCursorPosY(windowHeight * 5.0f / 6.0f - buttonNormalSize.y * 0.5f);
    
    stepX = (windowWidth) / 3 - (buttonNormalSize.x) * 0.5f;
    ImGui::SetCursorPosX(stepX);

    if(ImGui::Button("aply", buttonNormalSize)){
        game->applySettingChanges(sensitivity, this->currentResolutionIndex);
    }
    ImGui::SameLine();

    stepX += (windowWidth) / 3;
    ImGui::SetCursorPosX(stepX);

    if(ImGui::Button("quit", buttonNormalSize)){
        this->game->requestChangeState(stateApp::EXIT, stateAction::NONE);
    }
}