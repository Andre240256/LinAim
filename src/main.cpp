#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <chrono>
#include <iostream>

#include "../include/shader.hpp"
#include "../include/camera.hpp"
#include "../include/player.hpp"
#include "../include/ball.hpp"
#include "../include/crosshair.hpp"
#include "../include/grid.hpp"
#include "../include/bullet.hpp"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow * window, int width, int size);
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow * window, int button, int actions, int mods);

void processInput(GLFWwindow * window, float deltaTime);

float genRandomFloat(float inf, float max);
int genRandomInt(int inf, int max);

unsigned int loadUBO();

std::random_device rd;
std::mt19937 gen(rd());

unsigned int Shader::currentProgramID = 0;
Player player(glm::vec3(0.0f, 1.5f, 10.0f));
std::vector<Bullet*> bullets;
std::vector<Ball *> balls;

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
    // glEnable(GL_CULL_FACE);
    glViewport(0, 0, WIDTH, HEIGHT);

    unsigned int matricesUBO = loadUBO();
    
    Grid grid;
    for(int i = 0; i < 3; i++)
        balls.push_back(
            new Ball(
                glm::vec3(
                        genRandomFloat(-10.0f, 10.0f),
                        3.0f + genRandomFloat(-1.0f, 3.0f),
                        0.0f
                        )
                )
            );
    Crosshair crosshair;

    grid.bindUniformBlock(0, "Matrices");

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    int i = 0;
    bool destroyed = false;
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // if(i++ % 40 == 0)
        //     std::cout << "FPS: " << 1.0/deltaTime << "\n", i = 1;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, deltaTime);

        float aspectio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

        view = player.getViewMat();
        projection = glm::perspective(glm::radians(player.zoom), aspectio, 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for(auto it = bullets.begin(); it != bullets.end();){
            Bullet * b = *it;
            if(b->active)
                b->draw(), it++;
            else 
                it = bullets.erase(it);
        }

        for(Ball * b : balls){
            if(b->active)
                b->draw();
            else{
                b->active = true;
                b->pos = glm::vec3(
                    genRandomFloat(-10.0f, 10.0f),
                    3.0f + genRandomFloat(-1.0f, 3.0f),
                    0.0f
                );
            }
        }

        for(auto it = bullets.begin(); it != bullets.end(); it++){
            Bullet* b = *it;
            glm::vec3 oldPos = b->updatePos(window, deltaTime);

            if(!(b->active)){
                continue;
            }
  
            for(auto itr = balls.begin(); itr != balls.end(); ++itr)
            {
                Ball* ball = *itr;
                b->checkCollision(b->pos, oldPos, *ball);   
            }
        }
        

        grid.drawCell();
        crosshair.draw(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow * window, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    player.updatePos(window, deltaTime);
}

unsigned int loadUBO()
{
    unsigned int ubo;
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);

    return ubo;
}

float genRandomFloat(float inf, float max)
{
    std::uniform_real_distribution floatDist(inf, max);
    return floatDist(gen);
}

int genRandomInt(int inf, int max)
{
    std::uniform_int_distribution intDis(inf, max);
    return intDis(gen);
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
{
    player.updateDir(window, xposIn, yposIn);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    player.updateZoom(yoffset);
}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
       bullets.push_back(new Bullet(player.Pos + player.Front * 1.0f, player.Front));
       bullets[bullets.size() - 1]->bindUniformBlock(0, "Matrices"); 
    }
}