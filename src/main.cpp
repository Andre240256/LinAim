#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <chrono>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "stb_image/stb_image.h"

#include "core/configUI.hpp"
#include "core/shader.hpp"
#include "core/camera.hpp"
#include "core/player.hpp"
#include "entities/ball.hpp"
#include "entities/crosshair.hpp"
#include "entities/grid.hpp"
#include "entities/bullet.hpp"
#include "core/skyBox.hpp"
#include "states/stateGame.hpp"
#include "states/stateStartMenu.hpp"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow * window, int width, int size);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow * window, int button, int actions, int mods);

unsigned int Shader::currentProgramID = 0;
StateGame * game = nullptr;
StateStartMenu * startmenu = nullptr;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LinAim", NULL, NULL);
    if(!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initializa glad" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    configUI::setup();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);

    stateApp currentState = stateApp::START_MENU;

    while(currentState != stateApp::EXIT)
        switch(currentState){
            case stateApp::GAME:
                if(game == nullptr){
                    game = new StateGame();
                    game->setWindow(window);
                }
                currentState = game->run();
                break;

            case stateApp::START_MENU:
                if(startmenu == nullptr){
                    startmenu = new StateStartMenu(window);
                }
                currentState = startmenu->run();
                break;
                
            case stateApp::EXIT:
                break;
        }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    delete game;
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
{
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
        ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
        return;
    }

    if(game != nullptr)
        game->player.updateDir(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    if(game != nullptr)
        game->player.updateZoom(yoffset);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) {
            return;
        }
    }

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && game != nullptr)
    {
       game->bullets.push_back(new Bullet(game->player.Pos + game->player.Front * 1.0f, game->player.Front));
    }
}