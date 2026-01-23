#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <vector>
#include <cmath>

#include "shader.hpp"

class Ball
{
public:
    glm::vec3 pos;
    glm::vec3 scale;
    bool active;

    unsigned int VAO;
    unsigned int indexCount;
    
    Ball(glm::vec3 startPos = glm::vec3(0.0f), int xSegments = 30, int ySegments = 30);

    void draw() const;

protected:
    glm::vec3 ballColor;
    Shader shader;
    
    glm::mat4 getModelMatrix() const;
private:
    unsigned int VBO, EBO;
    
    void setupBall(int xSegments, int ySegments);
};