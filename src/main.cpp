#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <chrono>
#include <iostream>
#include <thread>

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
#include "states/stateSettings.hpp"

void framebuffer_size_callback(GLFWwindow * window, int width, int size);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow * window, int button, int actions, int mods);

unsigned int Shader::currentProgramID = 0;

StateGame * game = nullptr;
StateStartMenu * startmenu = nullptr;
StateSettings * settings = nullptr;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    configUI::loadSettings();

    int count;
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * modes = glfwGetVideoModes(monitor, &count);

    int bestIndex = 0;

    for(int i = 0; i < count; i++){
        if(modes[i].refreshRate > modes[bestIndex].refreshRate){
            bestIndex = i;
        }
        else if(modes[i].refreshRate == modes[bestIndex].refreshRate){
            if(modes[i].width > modes[bestIndex].width)
                bestIndex = i;
        }
    }

    glfwWindowHint(GLFW_RED_BITS, modes[bestIndex].redBits);
    glfwWindowHint(GLFW_GREEN_BITS, modes[bestIndex].greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, modes[bestIndex].blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, modes[bestIndex].refreshRate);

    int width = modes[bestIndex].width;
    int height = modes[bestIndex].height;
    std::cout << width << " : " << height << std::endl;
    GLFWwindow * window = glfwCreateWindow(width, height, "LinAim", monitor, NULL);
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

    configUI::window = window;
    configUI::setup();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    stateApp currentState = stateApp::START_MENU;
    stateApp lastState = stateApp::START_MENU;

    game = new StateGame();
    game->setWindow(window);

    while(currentState != stateApp::EXIT){
        switch(currentState){
            case stateApp::GAME:
                if(game == nullptr){
                    game = new StateGame();
                    game->setWindow(window);
                }
                currentState = game->run();
                if(currentState == stateApp::SETTINGS)
                    lastState = stateApp::GAME;
                break;

            case stateApp::START_MENU:
                if(startmenu == nullptr){
                    startmenu = new StateStartMenu(window);
                }
                currentState = startmenu->run();
                if(currentState == stateApp::SETTINGS)
                    lastState = stateApp::START_MENU;
                break;

            case stateApp::SETTINGS:
                if(settings == nullptr){
                    settings = new StateSettings(window);
                }
                currentState = settings->run(lastState);
                break;
                
            case stateApp::EXIT:
                break;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    if(game != nullptr) delete game;
    if(startmenu != nullptr) delete startmenu;
    if(settings != nullptr) delete settings;

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
       game->shoot();
    }
}