#include "core/model.hpp"
#include <memory>

class Revolver : public Model{
public:
    static void initShader();

    Revolver(const std::string& modelPath);

    void draw();

    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
private:

    glm::mat4 getModelMatrix() const;
    static std::unique_ptr<Shader> shader;
};