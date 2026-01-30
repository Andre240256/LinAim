#include "states/stateSettings.hpp"

StateSettings::StateSettings(Game * ptrMaster) : game(ptrMaster)
{
    this->escPressedLastFrame = false;
    this->enterPressedLastFrame = false;
}

void StateSettings::run()
{
    this->sensitivity = this->game->getSettings().sensitivity;
    this->currentResolution = this->game->getSettings().currentResolution;
    this->currentResolutionIndex = this->game->getResolutionIndex();

    while(!glfwWindowShouldClose(this->game->getWindow()) && this->game->currentState == stateApp::SETTINGS)
    {
        glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        processInput();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport * viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        if(this->game->getFont() != nullptr) ImGui::PushFont(this->game->getFont());

        ImGui::Begin("settings menu", nullptr, this->game->getWindowFlagsDefault());
        {
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
            
            ImGui::Spacing();
            
            stepX = sliderCenter - sliderSize * 0.5f;
            ImGui::SetCursorPosX(stepX);
            ImGui::PushItemWidth(sliderSize);
            ImGui::SliderFloat("##sens_slider", &this->sensitivity , 0.0001f, 5.0f);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            stepX = boxCenter - boxSize * 0.5f;
            ImGui::SetCursorPosX(stepX);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushItemWidth(boxSize);
            if(ImGui::InputFloat("##sens_input", &this->sensitivity, 0.0f, 0.0f, "%.3f")){
                if(this->sensitivity < 0.0001f) this->sensitivity = 0.0001f;
                if(this->sensitivity > 5.0f) this->sensitivity = 5.0f;
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
            const char * previewValue = this->currentResolution.label.c_str();

            std::vector <Resolution>& resolutionsList = this->game->getAvailableResolutions();
            if(ImGui::BeginCombo("##Resolution_combo", previewValue)){
                for(int n = 0; n < resolutionsList.size(); n++){
                    const bool isSelected = (this->currentResolutionIndex == n);

                    if(ImGui::Selectable(resolutionsList[n].label.c_str(), isSelected)){
                        this->currentResolutionIndex = n;
                        this->currentResolution = resolutionsList[n];
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
                game->applySettingChanges(this->sensitivity, this->currentResolutionIndex);
            }
            ImGui::SameLine();

            stepX += (windowWidth) / 3;
            ImGui::SetCursorPosX(stepX);

            if(ImGui::Button("quit", buttonNormalSize)){
                glfwSetWindowShouldClose(this->game->getWindow(), true);
                this->game->currentState = stateApp::EXIT;
            }
        }
        ImGui::End();
        if (this->game->getFont()) ImGui::PopFont();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->game->getWindow());
    }

    if(glfwWindowShouldClose(this->game->getWindow())) this->game->currentState = stateApp::EXIT;
}

void StateSettings::processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        this->game->currentState = this->game->lastState;
    }

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        this->game->applySettingChanges(this->sensitivity, this->currentResolutionIndex);
    }
}


void StateSettings::processInput()
{
    // bool escPressed = glfwGetKey(this->game->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
    // if(escPressed && !escPressedLastFrame){
    //     escPressedLastFrame = escPressed;
    //     this->game->currentState = this->game->lastState;
    // }
    // escPressedLastFrame = escPressed;

    // bool enterPressed = glfwGetKey(this->game->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS;
    // if(enterPressed && !enterPressedLastFrame){
    //     enterPressedLastFrame = true;
    //     this->game->applySettingChanges(this->sensitivity, this->currentResolutionIndex);
    // }
    // enterPressedLastFrame = enterPressed;
}