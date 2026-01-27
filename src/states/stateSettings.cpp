#include "states/stateSettings.hpp"

StateSettings::StateSettings(GLFWwindow * window)
{
    this->window = window;
    this->escPressedLastFrame = false;
    this->sensibility = 0.3f;
}

stateApp StateSettings::run(stateApp lastState)
{
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

            ImGui::SliderFloat("sensibility", &this->sensibility , 0.0f, 10.0f);

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