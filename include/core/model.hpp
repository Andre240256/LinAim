#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/mesh.hpp"
#include "stb_image/stb_image.h"

class Shader;

class Model
{
public:
    //model data
    std::vector <Mesh> meshes;
    std::string directory;
    std::vector <Texture> textures_loaded;
    
    //CONSTRUCTOR
    Model(const std::string& path);
    
    void Draw(Shader & shader);

private:

    void loadModel(const std::string& path);
    void processNode(aiNode * node, const aiScene * scene);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector <Texture> loadMaterialTextures(aiMaterial * mat,
                                               aiTextureType type, 
                                               std::string typeName);
    unsigned int TextureFromFile(const std::string& name,
                                 const std::string& directory);
};