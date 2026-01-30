#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>

#include "stb_image/stb_image.h"
#include "core/shader.hpp"

class SkyBox
{
public:
    unsigned int ID, VAO, VBO;
    Shader shader;

    SkyBox(const std::string& texturePath);
    ~SkyBox();

    void draw();
    void bindUniformBlock(int gateway, const std::string& name) const;
    
private:
    void setupCubeMap(const std::string& texturePath);
    void setupCube();
};