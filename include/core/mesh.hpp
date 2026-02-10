#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/shader.hpp"

class Shader;

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 BiTangent;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    //mesh data
    std::vector <Vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector<Texture> textures;
    //render data
    unsigned int VAO, VBO, EBO;

    //constructor
    Mesh(std::vector <Vertex> vertices, std::vector <unsigned int> indices,
        std::vector <Texture> texture);

    void Draw(Shader &shader);
    
private:

    void setupMesh();
};