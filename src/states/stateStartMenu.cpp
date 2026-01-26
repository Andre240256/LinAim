#include "states/stateStartMenu.hpp"

StateStartMenu::StateStartMenu(GLFWwindow * window) 
{
    this->window = window;
    this->buttonNormalSize = {200.0f, 40.0f};
}

stateApp StateStartMenu::run()
{
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    stateApp currentState = stateApp::START_MENU;
    while(!glfwWindowShouldClose(this->window) && currentState == stateApp::START_MENU)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        currentState = processInput();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Start Menu");
        {
            float windowWidth = ImGui::GetWindowSize().x;
            float centerPosX = (windowWidth - this->buttonNormalSize.x) * 0.5f;

            ImGui::SetCursorPosX(centerPosX);
            if(ImGui::Button("Iniciar Jogo", this->buttonNormalSize)){
                currentState = stateApp::GAME;
                std::cout << "Botao clicado" << std::endl;
            }

            ImGui::Spacing();

            ImGui::SetCursorPosX(centerPosX);

            if(ImGui::Button("Sair do jogo", this->buttonNormalSize)){
                glfwSetWindowShouldClose(this->window, true);
                currentState = stateApp::EXIT;
                std::cout << "Saindo do jogo" << std::endl;
            }
        }
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->window);
    }

    if(glfwWindowShouldClose(this->window)) currentState = stateApp::EXIT;
    return currentState;
}

stateApp StateStartMenu::processInput()
{
    if(glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
        return stateApp::EXIT;
    }

    return stateApp::START_MENU;
}