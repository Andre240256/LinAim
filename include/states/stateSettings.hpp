#pragma once

#include "core/game.hpp"

class StateSettings : public State{
public:

    StateSettings(Game * ptrMaster);
    ~StateSettings() noexcept override;
    
    void init() override;
    void processInput() override;
    void update(float dt) override;
    void render() override;
    stateApp getType() override;

    void mouseCallback(GLFWwindow * window, double x, double y) override;
    void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override;
    void keyCallback(GLFWwindow * window, int key, int sancode, int action, int mods) override;
    void charCallback(GLFWwindow * window, unsigned int codepoint) override;

private:
    SettingData settingCopy;
    int currentResolutionIndex;
    
    void drawButtons();
};