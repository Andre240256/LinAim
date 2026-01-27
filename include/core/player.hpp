#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "camera.hpp"

class Player : public Camera{
public:
    Player(glm::vec3 PlayerPos = glm::vec3(0.0f, 0.0f, 3.0f));

    void updatePos(GLFWwindow * window, float deltaTime) override;

    void setSensibility(float sensibility);
private:
    const glm::vec3 Xaxis = glm::vec3(1.0f, 0.0f, 0.f);
    const glm::vec3 Yaxis = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 Zaxis = glm::vec3(0.0f, 0.0f, 1.0f);
};