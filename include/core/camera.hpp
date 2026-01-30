#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "configUI.hpp"

class Camera
{
public:
    Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));

    virtual void updatePos(GLFWwindow * window, float camSpeed);
    void updateDir(GLFWwindow * window, double xposIn, double yposIn);
    void updateZoom(double yoffset);

    void setSensitility(float sensitivity) { this->sensitivity = sensitivity; }

    glm::mat4 getViewMat();

    float yaw;
    float pitch;
    float fov;

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