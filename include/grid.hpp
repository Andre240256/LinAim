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
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;

    float gridSize;

    Grid(int quadSize = 2, int Nquads = 20);

    void draw() const;
    void drawCell();
private:
    Shader shader; 
    unsigned int VBO, VAO;
    int vertexCount;

    glm::mat4 getModelMatrix() const;
    void setupGrid(int quadSize, int Nquads);
};

Grid::Grid(int quadSize, int Nquads): shader("src/shaders/gridShader.vs", "src/shaders/gridShader.fs")
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
    shader.bindUniformBlock(0, "Matrices");
    setupGrid(quadSize, Nquads);
}

void Grid::setupGrid(int quadSize, int Nquads)
{
    std::vector <float> vertices;

    float step = static_cast<float> (quadSize);
    this->gridSize = static_cast<float> (quadSize * Nquads);
    
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

glm::mat4 Grid::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, this->pos);
    
    model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, this->scale);

    return model;
}

void Grid::draw() const
{   
    shader.use();
    shader.setMat4("model", this->getModelMatrix());
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, vertexCount);
    glBindVertexArray(0);
}

void Grid::drawCell()
{
    pos = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    float desloc = gridSize / 2.0f;
    //floor
    draw();

    //roof
    pos = glm::vec3(0.0f, 2 * desloc, 0.0f);
    draw();

    //walls
    rotation = glm::vec3(90.0f, 0.0f, 0.0f);
    pos = glm::vec3(0.0f, desloc, desloc);
    draw();
    pos.z = - desloc;
    draw();

    rotation = glm::vec3(0.0f, 0.0f, 90.0f);
    pos = glm::vec3(desloc, desloc, 0.0f);
    draw();
    pos.x = - desloc;
    draw();

    rotation = glm::vec3(0.0f);
    pos = glm::vec3(0.0f);
}