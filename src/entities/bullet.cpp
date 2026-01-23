#include "entities/bullet.hpp"

Bullet::Bullet(glm::vec3 startPos, glm::vec3 direction) : Ball(startPos)
{
    this->ballColor = glm::vec3(0.7f);
    this->scale = glm::vec3(0.05f);
    shader.setVec3("ballColor", ballColor);
    this->direction = glm::normalize(direction);
    this->velocity = 500.0f;
    this->lifeTime = 0;
}

glm::vec3 Bullet::updatePos(GLFWwindow * window, float deltaTime)
{
    lifeTime += deltaTime;
    if(lifeTime > 10.0f){
        active = false;
    }
    
    float distance = deltaTime * this->velocity;

    glm::vec3 oldPos = this->pos;
    this->pos += distance * this->direction;

    return oldPos;
}

bool Bullet::checkCollision(const glm::vec3& newPosition, const glm::vec3& pos, Ball& target)
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
    if(dist <= (this->scale.x + target.scale.x)){
        target.active = false;
        return true;
    }
    return false;
}