#pragma once

#include "overlays/overlay.hpp"
#include "core/game.hpp"


class OverlayFps : public Overlay{
public:
    OverlayFps(Game * ptrMaster);
    ~OverlayFps() noexcept override;

    void init() override;
    void update(float dt) override;
    void render() override;
    overlayType getType() override;

private:
    float clock;
    int fpsCount;
    int fps;

    void drawFpsStats();
};