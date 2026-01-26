#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "core/shader.hpp"

class Ball
{
public:
    static void initShader();

    glm::vec3 pos;
    glm::vec3 scale;
    bool active;

    unsigned int VAO;
    unsigned int indexCount;
    
    Ball(glm::vec3 startPos = glm::vec3(0.0f), int xSegments = 30, int ySegments = 30);
    ~Ball();
    
    void draw() const;

protected:
    glm::vec3 ballColor;
    
    glm::mat4 getModelMatrix() const;
    static Shader * shader;
    
private:

    unsigned int VBO, EBO;
    
    void setupBall(int xSegments, int ySegments);
};