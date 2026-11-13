#pragma once

#include <vector>
#include "Frame.hpp"

class Timeline {
private:
    std::vector<Frame> frames_;
    int currentFrameIndex_ = 0;
    int width_;
    int height_;

public:
    Timeline(int width, int height);

    void addFrame();
    Frame& getCurrentFrame();
    void nextFrame();
    void previousFrame();
    int getFrameCount() const;
    int getCurrentFrameIndex() const;
    void duplicateFrame();
};