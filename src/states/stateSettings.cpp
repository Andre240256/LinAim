#include "states/stateSettings.hpp"

StateSettings::StateSettings(GLFWwindow * window)
{
    this->window = window;
    this->escPressedLastFrame = false;
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

            float sliderSize = 900.0f;
            float stepX = (windowWidth - textSize) * 0.5f;
            float stepY = windowHeight / 6;

            ImGui::SetCursorPosX(stepX);
            ImGui::SetCursorPosY(stepY);
            ImGui::Text("Sensitivity");

            ImGui::Spacing();

            stepX = (windowWidth - sliderSize) * 0.5f;
            ImGui::SetCursorPosX(stepX);

            ImGui::PushItemWidth(sliderSize);
            ImGui::SliderFloat("", &this->sensitivity , 0.0f, 10.0f);
            ImGui::PopItemWidth();

            ImVec2 buttonNormalSize = {200.0f, 40.0f};
            ImGui::SetCursorPosY(windowHeight * 5.0f / 6.0f - buttonNormalSize.y * 0.5f);
            
            stepX = (windowWidth) / 3 - (buttonNormalSize.x) * 0.5f;
            ImGui::SetCursorPosX(stepX);

            if(ImGui::Button("aply", buttonNormalSize)){
                configUI::data.sensitivity = this->sensitivity;
                std::cout << "configuracoes alteradas" << std::endl;
                configUI::saveSettings();
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

stateApp StateSettings::processInput(stateApp lastState)
{
    bool escPressed = glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if(escPressed && !escPressedLastFrame){
        escPressedLastFrame = escPressed;
        return lastState;
    }
    escPressedLastFrame = escPressed;

    return stateApp::SETTINGS;
}