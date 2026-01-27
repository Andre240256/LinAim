#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <random>

#include "core/player.hpp"
#include "core/skyBox.hpp"

#include "entities/ball.hpp"
#include "entities/bullet.hpp"
#include "entities/crosshair.hpp"
#include "entities/grid.hpp"

#include "states/stateDefinitions.hpp"

class StateGame{
public:
    std::vector<Bullet *> bullets;
    std::vector<Ball *> balls;

    Player player;
    SkyBox skybox;
    Crosshair crosshair;
    Grid grid;

    StateGame();
    ~StateGame();

    stateApp run();
    void setWindow(GLFWwindow * window);

private:
    static std::mt19937& getGenerator();

    float deltaTime;
    float lastFrame;

    bool escPressedLastFrame;

    glm::mat4 view;
    glm::mat4 projection;

    GLFWwindow * window;
    int width, height;
    float aspect;
    unsigned int uboMatrices;

    void deltaTimeCalc();
    stateApp processInput();
    
    bool setBallsVector();

    void loadMatricesUBO(int gateway);

    glm::vec3 getRandomBallPosition() const;
    int getRandomInt(int a, int b) const;
    float getRandomFloat(float a, float b) const;
};