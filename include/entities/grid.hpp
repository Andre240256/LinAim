#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "core/shader.hpp"

class Grid{
public:
    static void initShader();
    
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;

    float gridSize;

    Grid(int quadSize = 2, int Nquads = 20);

    void draw() const;
    void drawCell();
private:

    static Shader * shader; 
    unsigned int VBO, VAO;
    int vertexCount;

    glm::mat4 getModelMatrix() const;
    void setupGrid(int quadSize, int Nquads);
};