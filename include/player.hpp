#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "camera.hpp"

glm::vec3 Xaxis = glm::vec3(1.0f, 0.0f, 0.f);
glm::vec3 Yaxis = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Zaxis = glm::vec3(0.0f, 0.0f, 1.0f);

class Player : public Camera{
public:
    Player(glm::vec3 PlayerPos = glm::vec3(0.0f, 0.0f, 3.0f));

    void updatePos(GLFWwindow * window, float deltaTime) override;
private:
};

Player::Player(glm::vec3 PlayerPos) : Camera(PlayerPos)
{
    this->sensitivity = 0.2;
    this->velocity = 4.0f;
}

void Player::updatePos(GLFWwindow * window, float deltaTime)
{
    float distance = deltaTime * this->velocity;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->Pos += distance * glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->Pos -= distance * glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->Pos += distance * glm::normalize(glm::cross(this->Front, Yaxis));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->Pos -= distance * glm::normalize(glm::cross(this->Front, Yaxis));
    }
}