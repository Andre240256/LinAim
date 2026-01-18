#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <vector>
#include <cmath>

class Shader;

class Ball
{
public:
    unsigned int VAO;
    unsigned int indexCount;
    
    Ball(int xSegments = 30, int ySegments = 30);

    void draw(const glm::mat4& model = glm::mat4(1.0f)) const;
    void getModelMatrix(glm::mat4& model, const glm::vec3 center, float radius);

    void bindUniformBlock(int gateWay, const std::string& name) const;

private:
    unsigned int VBO, EBO;
    Shader shader;

    void setupBall(int xSegments, int ySegments);
};

Ball::Ball(int xSegments, int ySegments) : shader("src/shaders/ballShader.vs", "src/shaders/ballShader.fs")
{
    setupBall(xSegments, ySegments);
}

void Ball::draw(const glm::mat4& model) const
{
    shader.use();
    shader.setMat4("model", model);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Ball::getModelMatrix(glm::mat4& model, const glm::vec3 center, float radius)
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, center);
    model = glm::scale(model, glm::vec3(radius));
}

void Ball::setupBall(int xSegments, int ySegments)
{
    const float PI = 3.14159265359f;
    std::vector<float> data;
    std::vector <unsigned int> indices;


    for(int y = 0; y <= ySegments; ++y)
        for(int x = 0; x <= xSegments; ++x){
            float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
            float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            data.push_back(xPos);
            data.push_back(yPos);
            data.push_back(zPos);
        }

    for(int y = 0; y < ySegments; ++y)
        for(int x = 0; x < xSegments; ++x){
            indices.push_back(y       * (xSegments + 1) + x);
            indices.push_back((y + 1) * (xSegments + 1) + x);
            indices.push_back((y + 1) * (xSegments + 1) + x + 1);

            indices.push_back(y       * (xSegments + 1) + x);
            indices.push_back((y + 1) * (xSegments + 1) + x + 1);
            indices.push_back(y       * (xSegments + 1) + x + 1);
        }

    indexCount = indices.size();

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Ball::bindUniformBlock(int gateway, const std::string& name) const
{
    glUniformBlockBinding(this->shader.ID, shader.getUniformBlockID(name), gateway);
}