#pragma once

#include "ball.hpp"

typedef enum updatePos{
    TIMEOUT,
    COLISION,
    NOTHING
} commands;

class Bullet : public Ball{
public:
    glm::vec3 direction;
    float lifeTime;

    Bullet(glm::vec3 startPos, glm::vec3 direction);

    glm::vec3 updatePos(GLFWwindow * window, float deltaTime);
    bool checkCollision(const glm::vec3& newPosition, const glm::vec3& pos, Ball& target);

private:
    float velocity;
    int countCheck;

};