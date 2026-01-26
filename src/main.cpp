#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <chrono>
#include <iostream>

#include "stb_image/stb_image.h"

#include "core/shader.hpp"
#include "core/camera.hpp"
#include "core/player.hpp"
#include "entities/ball.hpp"
#include "entities/crosshair.hpp"
#include "entities/grid.hpp"
#include "entities/bullet.hpp"
#include "core/skyBox.hpp"
#include "states/stateGame.hpp"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow * window, int width, int size);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow * window, int button, int actions, int mods);

unsigned int Shader::currentProgramID = 0;
StateGame * game = nullptr;

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
    // glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initializa glad" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);

    game = new StateGame();
    game->setWindow(window);
    game->run();
    
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
    game->player.updateDir(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    game->player.updateZoom(yoffset);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
       game->bullets.push_back(new Bullet(game->player.Pos + game->player.Front * 1.0f, game->player.Front));
    }
}