#include "fps_counter.hpp"

void fps_deltatime_calculate(FrameData* fpsCounter, float time_elapsed)
{
    fpsCounter->currentTime = time_elapsed;
    fpsCounter->deltaTime = fpsCounter->currentTime - fpsCounter->lastFpsCalcTime;
    fpsCounter->lastFpsCalcTime = fpsCounter->currentTime;
}

void fps_scuffed_calculate(FrameData* fpsCounter)
{
    const float second = 1.0f;
    const float secondsElapsedFromPrevious = (fpsCounter->currentTime + fpsCounter->overflowedFpsCalcTime) - fpsCounter->previousCurrentTime;

    if (second < secondsElapsedFromPrevious) {
        fpsCounter->displayFps = fpsCounter->frames;
        fpsCounter->frames = 0;
        fpsCounter->previousCurrentTime = fpsCounter->currentTime;
        fpsCounter->overflowedFpsCalcTime = secondsElapsedFromPrevious - second;
    }
}
