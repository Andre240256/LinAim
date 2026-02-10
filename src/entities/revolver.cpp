#include "entities/revolver.hpp"

std::unique_ptr<Shader> Revolver::shader = nullptr;

void Revolver::initShader(){
    if(shader == nullptr){
        shader = std::make_unique<Shader>("assets/shaders/revolver.vs", "assets/shaders/revolver.fs");
    }
}

Revolver::Revolver(const std::string& modelPath) : Model(modelPath)
{
    initShader();
    shader->bindUniformBlock(0, "Matrices"); 
    this->pos = glm::vec3(0.0);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(4.0f);
}

void Revolver::draw()
{
    if(shader == nullptr) return;

    shader->use();
    glm::mat4 model = this->getModelMatrix();
    shader->setMat4("model", model);

    this->Draw(*shader);
}

glm::mat4 Revolver::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, this->pos);
    
    model = glm::rotate(model, glm::radians(-this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, this->scale);

    return model;
}