#pragma once

#include <vector>
#include "Frame.hpp"
#include "InitializationMode.hpp"

class Timeline {
private:
    std::vector<Frame> frames_;
    int currentFrameIndex_ = 0;
    int width_;
    int height_;
    
public:
    Timeline(int width, int height, InitializationMode mode);

    void addFrame();
    void removeFrame(int index);
    Frame& getCurrentFrame();
    const Frame& getCurrentFrame() const;
    Frame& getFrame(int index);
    const Frame& getFrame(int index) const;
    void nextFrame();
    void previousFrame();
    int getFrameCount() const;
    int getCurrentFrameIndex() const;
    void duplicateFrame();
    Frame* getPreviousFrame();
    void goToFrame(int index);
    void moveFrame(int fromIndex, int toIndex);
    int getWidth() const;
    int getHeight() const;
};