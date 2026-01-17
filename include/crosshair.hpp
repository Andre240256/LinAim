#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include "shader.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Crosshair{
public:
    unsigned int VAO, VBO;
    Shader shader;
    int vertexCount;

    Crosshair();

    void draw(float screenWidht, float screenHeight);

private:
};

Crosshair::Crosshair() : shader("src/shaders/crosshair.vs", "src/shaders/crosshair.fs")
{
    std::vector<float> vertices;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    int segments = 32; 
    float radius = 0.5f; 

    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / (float)segments * 2.0f * M_PI;

        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        vertices.push_back(x);
        vertices.push_back(y);
    }

    vertexCount = vertices.size() / 2;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
}

void Crosshair::draw(float screenWidth, float screenHeight)
{
    glDisable(GL_DEPTH_TEST);

    this->shader.use();

    glm::mat4 projection = glm::ortho(
        -screenWidth / 2.0f, screenWidth / 2.0f,
        -screenHeight / 2.0f, screenHeight / 2.0f,
        -1.0f, 1.0f
    );

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));

    shader.setMat4("projection", projection);
    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(1.0f));

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
}