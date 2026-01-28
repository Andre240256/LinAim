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
#include <cmath>

#include "core/player.hpp"
#include "core/skyBox.hpp"

#include "entities/ball.hpp"
#include "entities/bullet.hpp"
#include "entities/crosshair.hpp"
#include "entities/grid.hpp"

#include "states/stateDefinitions.hpp"

struct GridSlot{
    glm::vec3 pos;
    bool occupied;

    GridSlot(glm::vec3 pos, bool occupied = false);
};

struct PlayerGameStats{
    unsigned int totalShots = 0;
    unsigned int targetsHit = 0;
};

class StateGame{
public:
    std::vector<Bullet *> bullets;
    std::vector<Ball *> balls;

    Player player;
    PlayerGameStats playerStats;
    SkyBox skybox;
    Crosshair crosshair;
    Grid grid;

    StateGame();
    ~StateGame();

    stateApp run();

    void freeGridSlot(int index);
    void setWindow(GLFWwindow * window);
    void shoot();

private:
    static std::mt19937& getGenerator();

    float deltaTime;
    float lastFrame;

    bool escPressedLastFrame;

    glm::mat4 view;
    glm::mat4 projection;

    GLFWwindow * window;
    unsigned int uboMatrices;

    std::vector<GridSlot> ballGrid;
    float gridSpacing;
    int gridRows;
    int gridCols; 

    void deltaTimeCalc();
    stateApp processInput();
    
    bool setBallsVector();
    void setBallgrid();
    int getRandomFreeSlot();

    void loadMatricesUBO(int gateway);

    glm::vec3 getRandomBallPosition() const;
    int getRandomInt(int a, int b) const;
    float getRandomFloat(float a, float b) const;
};