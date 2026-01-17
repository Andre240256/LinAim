#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <iostream>

#include "../include/shader.hpp"
#include "../include/camera.hpp"
#include "../include/ball.hpp"
#include "../include/crosshair.hpp"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow * window, int width, int size);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

void processInput(GLFWwindow * window, float deltaTime);

Camera camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initializa glad" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glViewport(0, 0, 800, 600);

    Shader shaderBall("src/shaders/ballShader.vs", "src/shaders/ballShader.fs");
    Ball ball;
    Crosshair crosshair;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, deltaTime);

        float aspectio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
        ball.getModelMatrix(model, glm::vec3(0.0f), 1.0f);
        view = camera.getViewMat();
        projection = glm::perspective(glm::radians(camera.zoom), aspectio, 0.1f, 100.0f);
        
        shaderBall.use();
        shaderBall.setMat4("model", model);
        shaderBall.setMat4("view", view);
        shaderBall.setMat4("projection", projection);

        ball.draw();
        crosshair.draw(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    camera.updatePos(window, deltaTime);
}

void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
{
    camera.updateDir(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    camera.updateZoom(yoffset);
}