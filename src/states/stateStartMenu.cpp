#include "states/stateStartMenu.hpp"

//constructor
//------------
StateStartMenu::StateStartMenu(Game * ptrMaster) : game(ptrMaster)
{
    this->escPressedLastFrame = false;
    this->buttonNormalSize = {200.0f, 40.0f};
    this->buttonBigSize = {400.0f, 80.0f};
}

void StateStartMenu::run()
{
    while(!glfwWindowShouldClose(this->game->getWindow()) && this->game->currentState == stateApp::START_MENU)
    {
        glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        processInput();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        if (this->game->getFont() != nullptr) ImGui::PushFont(this->game->getFont());
        ImGui::Begin("StartMenuOverlay", nullptr, this->game->getWindowFlagsDefault());
        
        this->drawButtons();

        ImGui::End();
        if (this->game->getFont() != nullptr) ImGui::PopFont();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->game->getWindow());
    }

    if(glfwWindowShouldClose(this->game->getWindow())) this->game->currentState = stateApp::EXIT;

}

void StateStartMenu::processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        this->game->currentState = stateApp::SETTINGS;
    }
}

//private functions
//---------------
void StateStartMenu::processInput()
{
    // bool escPressed = glfwGetKey(this->game->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
    // if(escPressed && !escPressedLastFrame){
    //     escPressedLastFrame = escPressed;
    //     this->game->currentState = stateApp::SETTINGS;
    // }
    // escPressedLastFrame = escPressed;
}

void StateStartMenu::drawButtons()
{
    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;

    float centerPosX = (windowWidth - this->buttonBigSize.x) * 0.5f;
    float centerPosY = windowHeight / 3.0f - this->buttonBigSize.y * 0.5f;

    ImGui::SetCursorPos(ImVec2(centerPosX, centerPosY));
    if(ImGui::Button("Play Game", this->buttonBigSize)){
        this->game->currentState = stateApp::FPS;
    }

    centerPosY += windowHeight / 6.0f;
    ImGui::SetCursorPos(ImVec2(centerPosX, centerPosY));
    if(ImGui::Button("Settings", this->buttonBigSize)){
        this->game->currentState = stateApp::SETTINGS;
    }

    float yStep = windowHeight / 6.0f - this->buttonBigSize.y - this->buttonNormalSize.y * 0.5f;
    ImGui::Dummy(ImVec2(0.0f, yStep));

    centerPosX = (windowWidth - this->buttonNormalSize.x) * 0.5f;
    ImGui::SetCursorPosX(centerPosX);
    if(ImGui::Button("quit", this->buttonNormalSize)){
        glfwSetWindowShouldClose(this->game->getWindow(), true);
        this->game->currentState = stateApp::EXIT;
    }
}