#include "core/player.hpp"

Player::Player(glm::vec3 PlayerPos) : Camera(PlayerPos) {}

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

void Player::setSensibility(float sensibility)
{
    this->sensitivity = sensibility;
}