#include "states/stateFps.hpp"

//constructor
GridSlot::GridSlot(glm::vec3 pos, bool occupied) :
pos(pos), occupied(occupied)
{}

//Constructor
StateFps::StateFps(Game * ptrMaster) :
skybox("assets/textures/skyCubeMap/"), game(ptrMaster), player(glm::vec3(0.0f, 3.0f, 10.0f))
{
    Ball::initShader();
    Grid::initShader();
    
    this->player.setSensitility(this->game->getSettings().sensitivity);
    this->escPressedLastFrame = false;

    loadMatricesUBO(0);
    setBallgrid();
    setBallsVector(); 
}

StateFps::~StateFps()
{
    for(auto it = bullets.begin(); it != bullets.end(); it++)
        delete *it;
    bullets.clear();
    
    for(auto it = balls.begin(); it != balls.end(); it++)
        delete *it;
    balls.clear();
}

//Public Functions
void StateFps::run() 
{
    glfwSetInputMode(this->game->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::mat4 view;
    glm::mat4 projection;

    int width = this->game->getSettings().currentResolution.width;
    int height = this->game->getSettings().currentResolution.height;
    float aspect;
    aspect = static_cast<float>(width) / static_cast<float>(height);
    float vFovRad = 2.0f * std::atan(std::tan(glm::radians(player.fov) / 2.0f) / aspect);
    projection = glm::perspective(vFovRad, aspect, 0.1f, 100.0f);


    this->player.setSensibility(this->game->getSettings().sensitivity);
    this->player.lastX = static_cast<float>(width) / 2.0f;
    this->player.lastY = static_cast<float>(height) / 2.0f;
    this->player.firstMouse = true;

    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;

    while(!glfwWindowShouldClose(this->game->getWindow()) && this->game->currentState == stateApp::FPS)
    {
        deltaTimeCalc();
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput();

        view = player.getViewMat();

        glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for(auto it = bullets.begin(); it != bullets.end();){
            Bullet * bullet = *it;
            if(bullet->active)
                bullet->draw(), it++;
            else{    
                delete *it;
                it = bullets.erase(it);
            }
        }

        for(Ball * ball : balls){
            if(ball->active)
                ball->draw();
            else{
                freeGridSlot(ball->currentSlot);
                int randomIndex = getRandomFreeSlot();
                ball->active = true;
                ball->currentSlot = randomIndex;
                ball->pos = this->ballGrid[randomIndex].pos;
                this->ballGrid[randomIndex].occupied = true;
                this->playerStats.targetsHit++;
                std::cout << "Total Targets hit: " << this->playerStats.targetsHit << std::endl;
            }
        }

        for(auto it = bullets.begin(); it != bullets.end(); it++){
            Bullet * bullet = *it;
            glm::vec3 oldPos = bullet->updatePos(this->game->getWindow(), deltaTime);

            if(!(bullet->active)){
                continue;
            }

            for(auto itr = balls.begin(); itr != balls.end(); itr++){
                Ball * ball = *itr;
                bullet->checkCollision(bullet->pos, oldPos, *ball);
            }
        }  

        grid.drawCell();
        skybox.draw();
        crosshair.draw(static_cast<float>(width), static_cast<float>(height));

        glfwSwapBuffers(this->game->getWindow());
        glfwPollEvents();
    }

    glfwSetInputMode(this->game->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void StateFps::freeGridSlot(int index)
{
    if(index >= 0 && index < ballGrid.size()){
        ballGrid[index].occupied = false;
    }
}

void StateFps::shoot()
{
    bullets.push_back(new Bullet(this->player.Pos + this->player.Front * 1.0f, this->player.Front));
    this->playerStats.totalShots++;
    std::cout << "Total shots: " << this->playerStats.totalShots << std::endl;
    fflush(stdout);
}

void StateFps::processKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        this->game->currentState = stateApp::SETTINGS;
    }
}

//Private Functions

void StateFps::deltaTimeCalc()
{
    float currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
}

void StateFps::processInput()
{
    player.updatePos(this->game->getWindow(), this->deltaTime);
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

        Ball * ball = new Ball(this->ballGrid[randomIndex].pos, randomIndex);
        if(ball == nullptr) return false;
        ballGrid[randomIndex].occupied = true;
        this->balls.push_back(ball);
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

//Depricated function not in use, we use *getRandomFreeSlot* 
//and then get the position of that random slot in the grid.
glm::vec3 StateFps::getRandomBallPosition() const
{
    bool validPos = false;
    int i = 0;
    glm::vec3 randomPos;
    while(!validPos && i < 100)
    {
        validPos = false;
        randomPos = glm::vec3 (
            getRandomFloat(-10.0f, 10.0f),
            3.0f + getRandomFloat(-1.0f, 3.0f),
            -5.0f
        );

        for(auto ball : this->balls){
            if(ball != nullptr)
                if(length(ball->pos - randomPos) > ball->scale.x * 2)
                    validPos = true;
        }

        i++;
    }

    return randomPos;
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