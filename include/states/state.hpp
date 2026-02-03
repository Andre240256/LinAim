#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

enum class stateApp {
    START_MENU,
    MENU,
    FPS,
    SETTINGS,
    EXIT,
    NONE,
};

enum class stateAction {
    POP,
    PUSH,
    CHANGE,
    NONE,
};

class Game;

class State {
public:
    State(Game * ptrGame);
    virtual ~State() = default;

    virtual void init() = 0;
    virtual void processInput() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual stateApp getType() = 0;

    virtual void mouseCallback(GLFWwindow * window, double x, double y) = 0;
    virtual void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) = 0;
    virtual void keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods) = 0;
    virtual void charCallback(GLFWwindow * window, unsigned int codepoint) = 0;

protected:
    Game * game;

    bool beginGameWindow(const std::string& title);
    void endGameWindow();
};