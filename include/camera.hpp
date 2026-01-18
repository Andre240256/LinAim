#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Camera
{
public:
    Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));

    virtual void updatePos(GLFWwindow * window, float camSpeed);
    void updateDir(GLFWwindow * window, double xposIn, double yposIn);
    void updateZoom(double yoffset);

    glm::mat4 getViewMat();

    float yaw;
    float pitch;
    float zoom;

    float lastX, lastY;
    bool firstMouse;

    glm::vec3 Pos;
    glm::vec3 Front;
    glm::vec3 Up;

protected:

    float sensitivity;
    float velocity;
private:
};

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
    this->yaw = -90.0f, this->pitch = 0.0f;

    this->zoom = 45.0f;

    this->Pos = cameraPos;
    this->Front = cameraFront;
    this->Up = cameraUp;

    this->sensitivity = 0.3f;
    this->velocity = 2.5f;

    this->lastX = 640.0f;
    this->lastY = 360.0f;
    this->firstMouse = true;
}


void Camera::updatePos(GLFWwindow * window, float deltaTime)
{
    float distance = deltaTime * velocity;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        this->Pos += distance * this->Front;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        this->Pos -= distance * this->Front;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        this->Pos += distance * glm::normalize(glm::cross(this->Front, this->Up));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        this->Pos -= distance * glm::normalize(glm::cross(this->Front, this->Up));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        this->Pos += distance * this->Up;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
        this->Pos -= distance * this->Up;
    }
}

void Camera::updateDir(GLFWwindow * window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos, lastY = ypos;

    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    this->Front = glm::normalize(direction);
}

void Camera::updateZoom(double yoffset)
{
    this->zoom -= static_cast<float>(yoffset);
    if(zoom < 1.0f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}

glm::mat4 Camera::getViewMat()
{
    return glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
}