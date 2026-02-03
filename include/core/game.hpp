#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/configUI.hpp"
#include "states/state.hpp"

class StateFps;
class StateSettings;
class StateStartMenu;

class Game {
public:
    std::vector <State *> states;
    stateApp nextState = stateApp::NONE;
    stateAction nextAction = stateAction::NONE;

    Game();
    ~Game();

    void run();
    void applySettingChanges(float sensitivity, int currentResolutionIndex);
    void requestChangeState(stateApp nextState, stateAction action);

    //getters
    SettingData& getSettings() { return this->settings; };
    SettingData getSettingsCopy() const { return this->settings; };
    GLFWwindow * getWindow() const { return this->window; };
    ImFont * getFont() const { return this->configUI.mainFont; };
    ImGuiWindowFlags getWindowFlagsDefault() const { return this->configUI.windowFlagsDefault; };
    std::vector <Resolution>& getAvailableResolutions() {return this->availableResolutions; }
    int getResolutionIndex() const { return this->currentResolutionIndex; };

private:

    bool shouldClose;
    SettingData settings;
    ConfigUI configUI;
    GLFWwindow * window;
    std::vector <Resolution> availableResolutions;
    int currentResolutionIndex;
    const std::string settingsPath = "assets/data/config.json";

    void beginImGuiFrame();
    void endImGuiFrame();

    State * createState(stateApp nextState);
    void popState();
    void pushState();
    void changeState();

    void pollEvents();

    int loadOpenGL();
    int loadGlad();
    
    int loadImGui();
    int shutdownImGui();
    int clearStates();
    
    int loadCallBacks();
    int loadSettings(); 
    int saveSettings();
    int loadResolutions();
    int setup();

    void framebuffer_size_callback(GLFWwindow * window, int width, int height);
    void mouse_callback(GLFWwindow * window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
    void key_callback(GLFWwindow * widnow, int key, int scancode, int action, int mods);
    void char_callback(GLFWwindow * window, unsigned int codepoint);

    static void framebuffer_size_callback_wrapper(GLFWwindow * window, int width, int height);
    static void mouse_callback_wrapper(GLFWwindow * window, double xpos, double ypos);
    static void mouse_button_callback_wrapper(GLFWwindow * window, int button, int actions, int mods);
    static void key_callback_wrapper(GLFWwindow * window, int key, int scancode, int action, int mods);
    static void char_callback_wrapper(GLFWwindow * window, unsigned int codepoint);
};
