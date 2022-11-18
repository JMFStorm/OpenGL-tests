#include <GLFW/glfw3.h>
#include "fps_counter.hpp"

void fps_frames_increment(FrameData* fpsCounter, const unsigned int frames) {
    fpsCounter->frames += frames;
}

void fps_deltatime_calculate(FrameData* fpsCounter) {
    fpsCounter->currentTime = (float)glfwGetTime();
    fpsCounter->deltaTime = fpsCounter->currentTime - fpsCounter->lastFpsCalcTime;
    fpsCounter->lastFpsCalcTime = fpsCounter->currentTime;
}

void fps_scuffed_calculate(FrameData* fpsCounter) {
    const float second = 1.0f;
    const float secondsElapsedFromPrevious = (fpsCounter->currentTime + fpsCounter->overflowedFpsCalcTime) - fpsCounter->previousCurrentTime;
    if (second < secondsElapsedFromPrevious) {
        fpsCounter->displayFps = fpsCounter->frames;
        fpsCounter->frames = 0;
        fpsCounter->previousCurrentTime = fpsCounter->currentTime;
        fpsCounter->overflowedFpsCalcTime = secondsElapsedFromPrevious - second;
    }
}
