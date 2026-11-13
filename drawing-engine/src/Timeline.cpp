#include "Timeline.hpp"
#include "Frame.hpp"

Timeline::Timeline(int width, int height)
    : width_(width), height_(height)
{
    frames_.emplace_back(Frame(width_, height_));
}

void Timeline::addFrame() {
    frames_.emplace_back(Frame(width_, height_));
    currentFrameIndex_ = frames_.size()-1;
}

Frame& Timeline::getCurrentFrame() {
    return frames_.at(currentFrameIndex_);
}

void Timeline::nextFrame() {
    if (currentFrameIndex_ < frames_.size()-1) {
        currentFrameIndex_ += 1;
    }
}

void Timeline::previousFrame() {
    if (currentFrameIndex_ > 0) {
        currentFrameIndex_ -= 1;
    }
}

int Timeline::getFrameCount() const {
    return frames_.size();
}

int Timeline::getCurrentFrameIndex() const {
    return currentFrameIndex_;
}

void Timeline::duplicateFrame() {
    frames_.insert(
        frames_.begin() + currentFrameIndex_ + 1,
        frames_.at(currentFrameIndex_)
    );

    currentFrameIndex_++;
}

Frame* Timeline::getPreviousFrame() {
    if (currentFrameIndex_ == 0) {
        return nullptr;
    }
    return &frames_.at(currentFrameIndex_-1);
}