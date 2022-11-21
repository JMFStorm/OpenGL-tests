struct FrameData {
    unsigned long frames;
    unsigned int displayFps;
    float previousCurrentTime;
    float currentTime;
    float lastFpsCalcTime;
    float deltaTime;
    float overflowedFpsCalcTime;
};

void fps_deltatime_calculate(FrameData* fpsCounter, float time_elapsed);
void fps_scuffed_calculate(FrameData* fpsCounter);
