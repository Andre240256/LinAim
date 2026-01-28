#include "core/camera.hpp"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
    this->yaw = -90.0f, this->pitch = 0.0f;

    this->fov = 71.1f;

    this->Pos = cameraPos;
    this->Front = cameraFront;
    this->Up = cameraUp;

    this->sensitivity = configUI::data.sensitivity;
    this->velocity = 4.0f;

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

    const float VALORANT_SCALE = 0.07f;

    yaw += xoffset * this->sensitivity * VALORANT_SCALE;
    pitch += yoffset * this->sensitivity * VALORANT_SCALE;

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

glm::mat4 Camera::getViewMat()
{
    return glm::lookAt(this->Pos, this->Pos + this->Front, this->Up);
}