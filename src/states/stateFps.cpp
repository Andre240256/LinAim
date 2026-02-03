#include "states/stateFps.hpp"

//constructor
GridSlot::GridSlot(glm::vec3 pos, bool occupied) :
pos(pos), occupied(occupied) {}

//Constructor
StateFps::StateFps(Game * ptrMaster) :
 State(ptrMaster), skybox("assets/textures/skyCubeMap/"), player(glm::vec3(0.0f, 3.0f, 10.0f))
{
    Ball::initShader();
    Grid::initShader();
    this->loadMatricesUBO(0);
    this->init();
    this->setBallgrid();
    this->setBallsVector(); 
}

StateFps::~StateFps()
{
    bullets.clear();
    balls.clear();
}

void StateFps::init()
{
    glfwSetInputMode(this->game->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    this->player.setSensitivity(this->game->getSettings().sensitivity);

    Resolution currentResolution = this->game->getSettings().currentResolution;
    this->width = currentResolution.width;
    this->height = currentResolution.height;

    this->aspect = static_cast<float>(width) / static_cast<float> (height);
    this->hFov = 2.0f * std::atan(std::tan(glm::radians(this->player.fov)/ 2.0f) / this->aspect);
    this->projection = glm::perspective(hFov, aspect, 0.1f, 100.0f);

    this->player.lastX = static_cast<float>(width) / 2.0f;
    this->player.lastY = static_cast<float>(height) / 2.0f;
    this->player.firstMouse = true;

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void StateFps::processInput() {}

void StateFps::update(float dt)
{
    this->player.updatePos(this->game->getWindow(), dt);

    for(auto it = this->bullets.begin(); it != this->bullets.begin();){
        Bullet& bullet = **it;
        if(bullet.active) it++;
        else{
            it = this->bullets.erase(it);
        }
    }

    for(auto& ball : this->balls){
        if(!ball->active){
            int randomIndex = getRandomFreeSlot();
            freeGridSlot(ball->currentSlot);
            ball->active = true;
            ball->currentSlot = randomIndex;
            ball->pos = this->ballGrid[randomIndex].pos;
            this->ballGrid[randomIndex].occupied = true;
            this->playerStats.targetsHit++;
            std::cout << "Total Targets hit: " << this->playerStats.targetsHit << std::endl;
        }
    }

    for(auto it = this->bullets.begin(); it != bullets.end(); it++){
        Bullet& bullet = **it;
        glm::vec3 oldPos = bullet.updatePos(this->game->getWindow(), dt);

        if(!bullet.active) continue;

        for(auto itr = this->balls.begin(); itr != this->balls.end(); itr++){
            Ball& ball = **itr;
            bullet.checkCollision(bullet.pos, oldPos, ball);
        }
    }
}

void StateFps::render()
{
    this->view = this->player.getViewMat();

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(auto& bullet : this->bullets){
        if(bullet->active)
            bullet->draw();
    }

    for(auto& ball : this->balls){
        if(ball->active)
            ball->draw();
    }

    grid.drawCell();
    skybox.draw();
    crosshair.draw(this->width, this->height);
}

stateApp StateFps::getType()
{
    return stateApp::FPS;
}

void StateFps::mouseCallback(GLFWwindow * window, double x, double y)
{
    this->player.updateDir(window, x, y);
}

void StateFps::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glm::vec3 startPos = this->player.Pos + this->player.Front * 0.1f;
        this->shoot();
    }
}

void StateFps::keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        this->game->requestChangeState(stateApp::SETTINGS, stateAction::PUSH);
    }
}

void StateFps::charCallback(GLFWwindow * window, unsigned int codepoint) {}

//private functions
//------------------
void StateFps::freeGridSlot(int index)
{
    if(index >= 0 && index < ballGrid.size()){
        ballGrid[index].occupied = false;
    }
}

void StateFps::shoot()
{
    glm::vec3 startPos = this->player.Pos + this->player.Front * 0.1f;
    bullets.push_back(std::make_unique<Bullet>(startPos, this->player.Front));
    this->playerStats.totalShots++;
    std::cout << "Total shots: " << this->playerStats.totalShots << std::endl;
    fflush(stdout);
}


void StateFps::loadMatricesUBO(int gateway)
{
    glGenBuffers(1, &this->uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, gateway, this->uboMatrices);
}

void StateFps::setBallgrid()
{
    this->gridCols = 5;
    this->gridRows = 3;
    this->gridSpacing = 2.5f;

    this->ballGrid.clear();

    float totalWidth = (this->gridCols - 1) * this->gridSpacing;
    float totalHeight = (this->gridRows - 1) * this->gridSpacing;

    float startX = - totalWidth / 2.0f;
    float startY = + totalHeight / 2.0f;

    for(int y = 0; y < this->gridRows; y++){
        for(int x = 0; x < this->gridCols; x++){
            glm::vec3 Pos;
            Pos.x = startX + this->gridSpacing * x;
            Pos.y = startY + this->gridSpacing * y;
            Pos.z = - 10.0f + getRandomFloat(-1.0f, 1.0f);
            this->ballGrid.push_back(GridSlot(Pos));
        }
    }

}

//Setters
bool StateFps::setBallsVector()
{
    for(int i = 0; i < 3; i++){
        int randomIndex = getRandomFreeSlot();
        if(randomIndex == -1) return false;

        auto ball = std::make_unique<Ball>(this->ballGrid[randomIndex].pos, randomIndex);
        if(ball == nullptr) return false;
        this->balls.push_back(std::move(ball));
        ballGrid[randomIndex].occupied = true;
    }
    return true;
}

//Getters
std::mt19937& StateFps::getGenerator()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    return gen;
}

int StateFps::getRandomFreeSlot()
{
    std::vector <int> freeIndices;
    for(int i = 0; i < this->ballGrid.size(); i++){
        if(!ballGrid[i].occupied){
            freeIndices.push_back(i);
        }
    }

    if(freeIndices.empty()) return -1;

    int randomIndex = rand() % freeIndices.size();
    return freeIndices[randomIndex];
}

int StateFps::getRandomInt(int a, int b) const
{
    std::uniform_int_distribution intDis(a, b);
    return intDis(getGenerator());
}

float StateFps::getRandomFloat(float a, float b) const
{
    std::uniform_real_distribution floatDis(a, b);
    return floatDis(getGenerator());
}