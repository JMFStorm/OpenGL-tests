struct FrameData {
    unsigned long frames;
    unsigned int displayFps;
    float previousCurrentTime;
    float currentTime;
    float lastFpsCalcTime;
    float deltaTime;
    float overflowedFpsCalcTime;
};

void fps_frames_increment(FrameData* fpsCounter, const unsigned int frames);
void fps_deltatime_calculate(FrameData* fpsCounter);
void fps_scuffed_calculate(FrameData* fpsCounter);
