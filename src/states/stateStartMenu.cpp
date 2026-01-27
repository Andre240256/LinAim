#include "states/stateStartMenu.hpp"

StateStartMenu::StateStartMenu(GLFWwindow * window) 
{
    this->window = window;
    this->escPressedLastFrame = false;
    this->buttonNormalSize = {200.0f, 40.0f};
    this->buttonBigSize = {400.0f, 80.0f};
}

stateApp StateStartMenu::run()
{
    stateApp currentState = stateApp::START_MENU;
    while(!glfwWindowShouldClose(this->window) && currentState == stateApp::START_MENU)
    {
        glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        currentState = processInput();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        if (configUI::MainFont) ImGui::PushFont(configUI::MainFont);

        ImGui::Begin("StartMenuOverlay", nullptr, configUI::windowFlags);
        {
            float windowWidth = ImGui::GetWindowSize().x;
            float centerPosX = (windowWidth - this->buttonBigSize.x) * 0.5f;
            float windowHeight = ImGui::GetWindowSize().y;
            float centerPosY = windowHeight / 3 - this->buttonBigSize.y * 0.5f;

            ImGui::SetCursorPosX(centerPosX);
            ImGui::SetCursorPosY(centerPosY);
            if(ImGui::Button("Play Game", this->buttonBigSize)){
                currentState = stateApp::GAME;
            }

            centerPosY += windowHeight / 6;
            ImGui::SetCursorPosY(centerPosY);
            ImGui::SetCursorPosX(centerPosX);

            if(ImGui::Button("Settings", this->buttonBigSize)){
                currentState = stateApp::SETTINGS;
            }

            float yStep = windowHeight / 6 - this->buttonBigSize.y - this->buttonNormalSize.y * 0.5f;
            ImGui::Dummy(ImVec2(0.0f, yStep));

            centerPosX = (windowWidth - this->buttonNormalSize.x) * 0.5f;
            ImGui::SetCursorPosX(centerPosX);
            if(ImGui::Button("quit", this->buttonNormalSize)){
                glfwSetWindowShouldClose(this->window, true);
                currentState = stateApp::EXIT;
            }
        }
        ImGui::End();
        if (configUI::MainFont) ImGui::PopFont();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->window);
    }

    if(glfwWindowShouldClose(this->window)) currentState = stateApp::EXIT;
    return currentState;
}

stateApp StateStartMenu::processInput()
{
    bool escPressed = glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if(escPressed && !escPressedLastFrame){
        escPressedLastFrame = escPressed;
        return stateApp::SETTINGS;
    }
    escPressedLastFrame = escPressed;

    return stateApp::START_MENU;
}