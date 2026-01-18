#pragma once

#include <GLFW/glfw3.h>

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
    bool active;

    Bullet(glm::vec3 startPos, glm::vec3 direction);

    commands updatePos(GLFWwindow * window, float deltaTime, const Ball& target);
private:
    float velocity;
    int countCheck;

    bool checkCollision(const glm::vec3& newPosition, const glm::vec3& pos, const Ball& target);
};

Bullet::Bullet(glm::vec3 startPos, glm::vec3 direction) : Ball(startPos)
{
    this->ballColor = glm::vec3(0.7f);
    this->scale = glm::vec3(0.05f);
    shader.setVec3("ballColor", ballColor);
    this->direction = glm::normalize(direction);
    this->velocity = 500.0f;
    this->lifeTime = 0;
}

commands Bullet::updatePos(GLFWwindow * window, float deltaTime, const Ball& target)
{
    lifeTime += deltaTime;
    if(lifeTime > 10.0f){
        active = false;
        return TIMEOUT;
    }
    
    float distance = deltaTime * this->velocity;

    glm::vec3 oldPos = this->pos;
    this->pos += distance * this->direction;
    if(checkCollision(pos, oldPos, target)){
        active = false;
        return COLISION;
    }

    return NOTHING;
}

bool Bullet::checkCollision(const glm::vec3& newPosition, const glm::vec3& pos, const Ball& target)
{
    glm::vec3 vecToTarget = target.pos - pos;
    glm::vec3 bulletPath = newPosition - pos;

    float pathLength = glm::length(bulletPath);
    if(pathLength == 0.0f) return false;

    float t = glm::dot(vecToTarget, direction);

    glm::vec3 closestPoint;
    if(t <= 0){
        closestPoint = pos;
    }
    else if(t >= pathLength){
        closestPoint = newPosition;
    }
    else{
        closestPoint = pos + direction * t;
    }

    float dist = glm::distance(closestPoint, target.pos);
    if(dist <= (this->scale.x + target.scale.x))
        return true;
    return false;
}