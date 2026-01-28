#include "states/stateSettings.hpp"

StateSettings::StateSettings(GLFWwindow * window)
{
    this->window = window;
    this->escPressedLastFrame = false;
    this->enterPressedLastFrame = false;
}

stateApp StateSettings::run(stateApp lastState)
{
    this->sensitivity = configUI::data.sensitivity;

    stateApp currentState = stateApp::SETTINGS;
    while(!glfwWindowShouldClose(this->window) && currentState == stateApp::SETTINGS)
    {
        glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        currentState = processInput(lastState);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport * viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        if(configUI::MainFont) ImGui::PushFont(configUI::MainFont);

        ImGui::Begin("settings menu", nullptr, configUI::windowFlags);
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

            ImVec2 buttonNormalSize = {200.0f, 40.0f};
            ImGui::SetCursorPosY(windowHeight * 5.0f / 6.0f - buttonNormalSize.y * 0.5f);
            
            stepX = (windowWidth) / 3 - (buttonNormalSize.x) * 0.5f;
            ImGui::SetCursorPosX(stepX);

            if(ImGui::Button("aply", buttonNormalSize)){
                applySettingsChanges();
            }
            ImGui::SameLine();

            stepX += (windowWidth) / 3;
            ImGui::SetCursorPosX(stepX);

            if(ImGui::Button("quit", buttonNormalSize)){
                glfwSetWindowShouldClose(window, true);
                currentState = stateApp::EXIT;
            }
        }
        ImGui::End();
        if (configUI::MainFont) ImGui::PopFont();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->window);
    }

    if(glfwWindowShouldClose(window)) currentState = stateApp::EXIT;
    return currentState;
}

int StateSettings::applySettingsChanges()
{
    configUI::data.sensitivity = this->sensitivity;
    std::cout << "configuracoes alteradas" << std::endl;
    configUI::saveSettings();

    return 1;
}

stateApp StateSettings::processInput(stateApp lastState)
{
    bool escPressed = glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if(escPressed && !escPressedLastFrame){
        escPressedLastFrame = escPressed;
        return lastState;
    }
    escPressedLastFrame = escPressed;

    bool enterPressed = glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS;
    if(enterPressed && !enterPressedLastFrame){
        enterPressedLastFrame = true;
        applySettingsChanges();
        return stateApp::SETTINGS;
    }
    enterPressedLastFrame = enterPressed;

    return stateApp::SETTINGS;
}