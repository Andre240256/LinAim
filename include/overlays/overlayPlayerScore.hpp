#pragma once

#include "overlays/overlay.hpp"

class Game;
enum class overlayType;

class OverlayPlayerScore : public Overlay{
public:

    OverlayPlayerScore(Game * ptrMaster);
    ~OverlayPlayerScore() noexcept override;

    void init() override;
    void update(float dt) override;
    void render() override;
    overlayType getType() override;
    
private:
    int targetHits = 0;
    int shoots = 0;

    void drawPlayerScore();
};