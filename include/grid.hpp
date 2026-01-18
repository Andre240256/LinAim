#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "shader.hpp"

class Grid{
public:
    Grid(int quadSize = 3, int Nquads = 30);

    void draw(const glm::mat4& model = glm::mat4(1.0f)) const;

    void bindUniformBlock(int gateway, const std::string& name) const;
private:
    Shader shader; 
    unsigned int VBO, VAO;
    int vertexCount;
    void setupGrid(int quadSize, int Nquads);
};

Grid::Grid(int quadSize, int Nquads): shader("src/shaders/gridShader.vs", "src/shaders/gridShader.fs")
{
    setupGrid(quadSize, Nquads);
}

void Grid::setupGrid(int quadSize, int Nquads)
{
    std::vector <float> vertices;

    float step = static_cast<float> (quadSize);
    float gridSize = static_cast<float> (quadSize * Nquads);
    
    for(int i = 0; i <= Nquads; i++){
        float currentPos = - gridSize / 2.0f + i * step;

        vertices.push_back(currentPos);
        vertices.push_back(0.0f);
        vertices.push_back(- gridSize / 2.0f);

        vertices.push_back(currentPos);
        vertices.push_back(0.0f);
        vertices.push_back(gridSize / 2.0f);

        vertices.push_back(- gridSize / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(currentPos);

        vertices.push_back(gridSize / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(currentPos);
    }

    this->vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Grid::draw(const glm::mat4& model) const
{
    shader.use();
    shader.setMat4("model", model);
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

void Grid::bindUniformBlock(int gateway, const std::string& name) const
{
    glUniformBlockBinding(shader.ID, shader.getUniformBlockID(name), gateway);
}