struct FpsCounter {
    unsigned long frames;
    unsigned int displayFps;
    float previousCurrentTime;
    float currentTime;
    float lastFpsCalcTime;
    float deltaTime;
    float overflowedFpsCalcTime;
};

void fps_frames_increment(FpsCounter* fpsCounter, const unsigned int frames);
void fps_deltatime_calculate(FpsCounter* fpsCounter);
void fps_scuffed_calculate(FpsCounter* fpsCounter);
