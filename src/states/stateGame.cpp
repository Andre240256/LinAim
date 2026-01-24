#include "states/stateGame.hpp"

//Constructor
StateGame::StateGame() :
skybox("assets/textures/skyCubeMap/"), player(glm::vec3(0.0f, 1.5f, 10.0f))
{
    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;

    loadMatricesUBO(0);
    setBallsVector(); 
}

//Public Functions
stateApp StateGame::run() 
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

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
            else    
                it = bullets.erase(it);
        }

        for(Ball * ball : balls){
            if(ball->active)
                ball->draw();
            else{
                ball->active = true;
                ball->pos = getRandomBallPosition();
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

    return currentState;
}

void StateGame::setWindow(GLFWwindow * window)
{
    this->window = window;
    glfwGetWindowSize(this->window, &this->width, &this->height);
    this->aspect = static_cast<float>(this->width) / static_cast<float>(this->height);
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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->window, true);
        return stateApp::EXIT;
    }
    player.updatePos(this->window, this->deltaTime);
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

//Setters
bool StateGame::setBallsVector()
{
    for(int i = 0; i < 3; i++){
        Ball * ball = new Ball(getRandomBallPosition());
        if(!ball) return false;
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

glm::vec3 StateGame::getRandomBallPosition() const
{
    return glm::vec3 (
        getRandomFloat(-10.0f, 10.0f),
        3.0f + getRandomFloat(-1.0f, 3.0f),
        -5.0f
    );
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