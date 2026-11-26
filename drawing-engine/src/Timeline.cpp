#include "Timeline.hpp"
#include "Frame.hpp"
#include <stdexcept>
#include <utility>
#include "Layer.hpp"


Timeline::Timeline(int width, int height, InitializationMode mode)
    : width_(width), height_(height)
{
    if (mode == InitializationMode::Default) {
        frames_.emplace_back(width_, height_);
    }
}

void Timeline::addFrame() {
    frames_.emplace_back(Frame(width_, height_));
    currentFrameIndex_ = frames_.size()-1;
}

void Timeline::removeFrame(int index) {
    if (index < 0 || index >= frames_.size()) {
        throw std::out_of_range("Invalid frame index");
    }

    frames_.erase(frames_.begin() + index);

    if (index < currentFrameIndex_) {
        currentFrameIndex_--;
    }
    if (currentFrameIndex_ >= frames_.size()) {
        currentFrameIndex_ = frames_.size() - 1;
    }
}

Frame& Timeline::getCurrentFrame() {
    return frames_.at(currentFrameIndex_);
}

const Frame& Timeline::getCurrentFrame() const {
    return frames_.at(currentFrameIndex_);
}

Frame& Timeline::getFrame(int index) {
    return frames_.at(index);
}

const Frame& Timeline::getFrame(int index) const {
    return frames_.at(index);
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
    Frame duplicate(width_, height_);
    duplicate.getLayerStack() = frames_.at(currentFrameIndex_).getLayerStack();
    for (int index = 0; index <= duplicate.getLayerStack().getLayerIndexes(); ++index) {
        Layer& layer = duplicate.getLayerStack().getLayer(index);
        layer = layer.duplicate();
    }
    frames_.insert(
        frames_.begin() + currentFrameIndex_ + 1,
        std::move(duplicate)
    );

    currentFrameIndex_++;
}

Frame* Timeline::getPreviousFrame() {
    if (currentFrameIndex_ == 0) {
        return nullptr;
    }
    return &frames_.at(currentFrameIndex_-1);
}

void Timeline::goToFrame(int index) {
    if (index < frames_.size() && index >= 0) {
        currentFrameIndex_ = index;
    } 
}

void Timeline::moveFrame(int fromIndex, int toIndex) {
    int count = static_cast<int>(frames_.size());

    if (fromIndex < 0 || fromIndex >= count ||
        toIndex < 0 || toIndex >= count) {
        throw std::out_of_range("Invalid frame index");
    }

    if (fromIndex == toIndex) {
        return;
    }

    if (fromIndex < toIndex) {
        std::rotate(
            frames_.begin() + fromIndex,
            frames_.begin() + fromIndex + 1,
            frames_.begin() + toIndex + 1
        );
    } else {
        std::rotate(
            frames_.begin() + toIndex,
            frames_.begin() + fromIndex,
            frames_.begin() + fromIndex + 1
        );
    }

    if (currentFrameIndex_ == fromIndex) {
    currentFrameIndex_ = toIndex;
    }
    else if (fromIndex < toIndex &&
            currentFrameIndex_ > fromIndex &&
            currentFrameIndex_ <= toIndex) {
        currentFrameIndex_--;
    }
    else if (fromIndex > toIndex &&
            currentFrameIndex_ >= toIndex &&
            currentFrameIndex_ < fromIndex) {
        currentFrameIndex_++;
    }
}

int Timeline::getWidth() const {
    return width_;
}

int Timeline::getHeight() const {
    return height_;
}
