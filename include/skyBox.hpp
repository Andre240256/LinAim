#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include "../vendor/stb_image.h"
#include "shader.hpp"

class SkyBox
{
public:
    unsigned int ID, VAO, VBO;
    Shader shader;

    SkyBox(const std::string& texturePath);

    void draw();
    void bindUniformBlock(int gateway, const std::string& name) const;
    
private:
    void setupCubeMap(const std::string& texturePath);
    void setupCube();
};

SkyBox::SkyBox(const std::string& texturePath) : shader("src/shaders/skyBox.vs", "src/shaders/skyBox.fs")
{
    shader.bindUniformBlock(0, "Matrices");
    setupCube();
    setupCubeMap(texturePath);
}

void SkyBox::draw()
{
    glDepthFunc(GL_LEQUAL);
    shader.use();

    glBindVertexArray(this->VAO);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

void SkyBox::setupCubeMap(const std::string& texturePath)
{
    std::vector <std::string> sufix = {
        "px", "nx", "py", "ny", "pz", "nz"
    };

    glGenTextures(1, &this->ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);

    int width, height, nrChannels;
    unsigned char * data;
    for(int i = 0; i < 6; i++){
        std::string completePath = texturePath + sufix[i] + ".png";
        data = stbi_load(completePath.c_str(), &width, &height, &nrChannels, 0);

        if(data){
            GLenum format;
            GLenum internalFormat;
            if(nrChannels == 1)
                format = GL_RED, internalFormat = GL_RED;
            else if(nrChannels == 3)
                format = GL_RGB, internalFormat = GL_SRGB;
            else if(nrChannels == 4)
                format = GL_RGBA, internalFormat = GL_SRGB_ALPHA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, 
                height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else{
            std::cout << "ERROR LOADING CUBE MAP FILE " << completePath << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::setupCube()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}