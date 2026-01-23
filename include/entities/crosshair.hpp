#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cmath>

#include "core/shader.hpp"

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