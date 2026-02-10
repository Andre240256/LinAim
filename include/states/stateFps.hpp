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

#include "core/game.hpp"
#include "core/player.hpp"

#include "entities/skyBox.hpp"
#include "entities/ball.hpp"
#include "entities/bullet.hpp"
#include "entities/crosshair.hpp"
#include "entities/grid.hpp"
#include "entities/revolver.hpp"

#include "states/state.hpp"

struct GridSlot{
    glm::vec3 pos;
    bool occupied;

    GridSlot(glm::vec3 pos, bool occupied = false);
};

struct PlayerGameStats{
    unsigned int totalShots = 0;
    unsigned int targetsHit = 0;
};

class StateFps : public State{
public:
    std::vector<std::unique_ptr<Bullet>> bullets;
    std::vector<std::unique_ptr<Ball>> balls;

    Player player;
    PlayerGameStats playerStats;
    SkyBox skybox;
    Crosshair crosshair;
    Grid grid;
    Revolver revolver;

    StateFps(Game * ptrMaster);
    ~StateFps() noexcept override;

    void init() override;
    void processInput() override;
    void update(float dt) override;
    void render() override;
    stateApp getType() override;

    void mouseCallback(GLFWwindow * window, double x, double y) override;
    void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override;
    void keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods) override;
    void charCallback(GLFWwindow * window, unsigned int codepoint) override;
    
private:

    void freeGridSlot(int index);
    void shoot();
    void processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

    static std::mt19937& getGenerator();

    float hFov;
    float aspect;

    float width;
    float height;

    glm::mat4 view;
    glm::mat4 projection;

    unsigned int uboMatrices;

    std::vector<GridSlot> ballGrid;
    float gridSpacing;
    int gridRows;
    int gridCols; 
    
    bool setBallsVector();
    void setBallgrid();
    int getRandomFreeSlot();

    void loadMatricesUBO(int gateway);

    glm::vec3 getRandomBallPosition() const;
    int getRandomInt(int a, int b) const;
    float getRandomFloat(float a, float b) const;
};