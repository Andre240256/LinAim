#include "states/stateGame.hpp"

//constructor
GridSlot::GridSlot(glm::vec3 pos, bool occupied) :
pos(pos), occupied(occupied)
{}

//Constructor
StateGame::StateGame() :
skybox("assets/textures/skyCubeMap/"), player(glm::vec3(0.0f, 1.5f, 10.0f))
{
    Ball::initShader();
    Grid::initShader();

    this->escPressedLastFrame = false;
    this->gridCols = 5;
    this->gridRows = 3;
    this->gridSpacing = 2.5f;

    loadMatricesUBO(0);
    setBallgrid();
    setBallsVector(); 
}

StateGame::~StateGame()
{
    for(auto it = bullets.begin(); it != bullets.end(); it++)
        delete *it;
    bullets.clear();
    
    for(auto it = balls.begin(); it != balls.end(); it++)
        delete *it;
    balls.clear();
}

//Public Functions
stateApp StateGame::run() 
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    this->player.setSensibility(configUI::data.sensitivity);
    this->player.lastX = 640.0f;
    this->player.lastY = 360.0f;
    this->player.firstMouse = true;

    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;

    stateApp currentState = stateApp::GAME;
    while(!glfwWindowShouldClose(window) && currentState == stateApp::GAME)
    {
        deltaTimeCalc();
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentState = processInput();

        view = player.getViewMat();
        projection = glm::perspective(player.zoom, aspect, 0.1f, 100.0f);

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
                ball->active = true;
                int randomIndex = getRandomFreeSlot();
                ball->currentSlot = randomIndex;
                this->ballGrid[randomIndex].occupied = true;
                ball->pos = this->ballGrid[randomIndex].pos;
            }
        }

        for(auto it = bullets.begin(); it != bullets.end(); it++){
            Bullet * bullet = *it;
            glm::vec3 oldPos = bullet->updatePos(window, deltaTime);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return currentState;
}

void StateGame::setWindow(GLFWwindow * window)
{
    this->window = window;
    glfwGetWindowSize(this->window, &this->width, &this->height);
    this->aspect = static_cast<float>(this->width) / static_cast<float>(this->height);
}

void StateGame::freeGridSlot(int index)
{
    if(index >= 0 && index < ballGrid.size()){
        ballGrid[index].occupied = false;
    }
}

//Private Functions

void StateGame::deltaTimeCalc()
{
    float currentFrame = glfwGetTime();
    this->deltaTime = currentFrame - this->lastFrame;
    this->lastFrame = currentFrame;
}

stateApp StateGame::processInput()
{
    bool escPressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if(escPressed && !escPressedLastFrame)
    {
        escPressedLastFrame = escPressed;
        return stateApp::SETTINGS;
    }
    escPressedLastFrame = escPressed;
    player.updatePos(this->window, this->deltaTime);

    // if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
    //     std::cout << "Free grid index : {";
    //     for(int i = 0; i < ballGrid.size(); i++){
    //         if(ballGrid[i].occupied == false){
    //             std::cout << i << ", ";
    //         }
    //     }
    //     std::cout << "}" << std::endl;
    // }
    return stateApp::GAME;
}

void StateGame::loadMatricesUBO(int gateway)
{
    glGenBuffers(1, &this->uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, gateway, this->uboMatrices);
}

void StateGame::setBallgrid()
{
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
bool StateGame::setBallsVector()
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
std::mt19937& StateGame::getGenerator()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    return gen;
}

int StateGame::getRandomFreeSlot()
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

glm::vec3 StateGame::getRandomBallPosition() const
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

int StateGame::getRandomInt(int a, int b) const
{
    std::uniform_int_distribution intDis(a, b);
    return intDis(getGenerator());
}

float StateGame::getRandomFloat(float a, float b) const
{
    std::uniform_real_distribution floatDis(a, b);
    return floatDis(getGenerator());
}