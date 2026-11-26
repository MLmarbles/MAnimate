#include "Editor.hpp"
#include "Color.hpp"
#include <algorithm>
#include <string>

Editor::Editor(int width, int height) 
    : timeline_(width, height),
      activeLayerIndex_(0),
      onionMode_(true),
      isPlaying_(false),
      fps_(12),
      brush_(),
      eraser_(),
      activeTool_(nullptr)
{}

void Editor::addFrame() {
    timeline_.addFrame();
    activeLayerIndex_ = 0;
}

void Editor::duplicateFrame() {
    timeline_.duplicateFrame();
    activeLayerIndex_ = 0;
}

void Editor::removeCurrentFrame() {
    timeline_.removeFrame(timeline_.getCurrentFrameIndex());
    activeLayerIndex_ = 0;
}

void Editor::nextFrame() {
    timeline_.nextFrame();
    activeLayerIndex_ = 0;
}

void Editor::previousFrame() {
    timeline_.previousFrame();
    activeLayerIndex_ = 0;
}

int Editor::getCurrentFrameIndex() const {
    return timeline_.getCurrentFrameIndex();
}

int Editor::getFrameCount() const {
    return timeline_.getFrameCount();
}

LayerStack& Editor::getLayerStack() {
    return timeline_.getCurrentFrame().getLayerStack();
}

void Editor::addLayer() {
    getLayerStack().addLayer();
    int layerIndexes = getLayerStack().getLayerIndexes();

    getLayerStack().getLayer(layerIndexes).getCanvas().clear(
        Color{0, 0, 0, 0}.pack()
    );

    activeLayerIndex_ = layerIndexes;
}

void Editor::removeCurrentLayer() {
    if (getLayerStack().getLayerIndexes() > 0) {
        getLayerStack().removeLayer(activeLayerIndex_);
        activeLayerIndex_ = std::min(
            activeLayerIndex_,
            getLayerStack().getLayerIndexes()
        );
    }
}

void Editor::nextLayer() {
    if (activeLayerIndex_ < getLayerStack().getLayerIndexes()) {
        activeLayerIndex_++;
    }
}

void Editor::previousLayer() {
    if (activeLayerIndex_ > 0) {
        activeLayerIndex_--;
    }
}

int Editor::getActiveLayerIndex() const {
    return activeLayerIndex_;
}

void Editor::changeLayerOpacity(float amount) {
    getActiveLayer().setOpacity(amount);
}

void Editor::toggleLayerVisibility() {
    getActiveLayer().toggleVisible();
}

Layer& Editor::getActiveLayer() {
    return getLayerStack().getLayer(activeLayerIndex_);
}

void Editor::moveLayerUp() {
    int newIndex = activeLayerIndex_ + 1;

    if (newIndex <= getLayerStack().getLayerIndexes()) {
        getLayerStack().moveLayer(activeLayerIndex_, newIndex);
        activeLayerIndex_ = newIndex;
    }
}

void Editor::moveLayerDown() {
    int newIndex = activeLayerIndex_ - 1;

    if (newIndex >= 0) {
        getLayerStack().moveLayer(activeLayerIndex_, newIndex);
        activeLayerIndex_ = newIndex;
    }
}

void Editor::moveFrameLeft() {
    int index = timeline_.getCurrentFrameIndex();

    if (index > 0) {
        timeline_.moveFrame(index, index - 1);
    }
}

void Editor::moveFrameRight() {
    int index = timeline_.getCurrentFrameIndex();

    if (index < timeline_.getFrameCount() - 1) {
        timeline_.moveFrame(index, index + 1);
    }
}

void Editor::undo() {
    Layer& layer = getLayerStack().getLayer(activeLayerIndex_);
    layer.getHistory().undo(layer.getCanvas());
}

void Editor::redo() {
    Layer& layer = getLayerStack().getLayer(activeLayerIndex_);
    layer.getHistory().redo(layer.getCanvas());
}

void Editor::onMouseDown(int x, int y, bool useEraser) {
    if (isPlaying_) return;
    Layer& layer = getActiveLayer();
    Canvas& canvas = layer.getCanvas();
    activeTool_ = useEraser ? static_cast<Tool*>(&eraser_) : &brush_;
    layer.getHistory().save(canvas);
    activeTool_->onMouseDown(canvas, x, y);
}

void Editor::onMouseMove(int x, int y) {
    if (isPlaying_) return;
    if (activeTool_ != nullptr) {
        activeTool_->onMouseMove(getActiveLayer().getCanvas(), x, y);
    }
}

void Editor::onMouseUp(int x, int y) {
    if (isPlaying_) return;
    if (activeTool_ != nullptr) {
        activeTool_->onMouseUp(getActiveLayer().getCanvas(), x, y);
        activeTool_ = nullptr;
    }
}

void Editor::togglePlayback() {
    isPlaying_ = !isPlaying_;
}

bool Editor::isPlaying() const {
    return isPlaying_;
}

void Editor::setFPS(int fps) {
    fps_ = std::clamp(fps, 1, 60);
}

int Editor::getFPS() const {
    return fps_;
}

void Editor::advancePlaybackFrame() {
    if (timeline_.getCurrentFrameIndex() == timeline_.getFrameCount() - 1) {
        timeline_.goToFrame(0);
    } else {
        timeline_.nextFrame();
    }
    activeLayerIndex_ = std::min(activeLayerIndex_, getLayerStack().getLayerIndexes());
}

void Editor::toggleOnionMode() {
    onionMode_ = !onionMode_;
}

bool Editor::isOnionMode() const {
    return onionMode_;
}

Canvas Editor::render() {
    Canvas composite = getLayerStack().composite();
    Canvas displayCanvas(composite.getWidth(), composite.getHeight());
    displayCanvas.clear(Color{255, 255, 255, 255}.pack());

    if (onionMode_ && !isPlaying_) {
        Frame* previous = timeline_.getPreviousFrame();
        if (previous != nullptr) {
            Canvas previousCanvas = previous->getLayerStack().composite();
            for (std::size_t y = 0; y < previousCanvas.getHeight(); ++y) {
                for (std::size_t x = 0; x < previousCanvas.getWidth(); ++x) {
                    Color top = Color::unpack(previousCanvas.getPixel(x, y));
                    Color bottom = Color::unpack(displayCanvas.getPixel(x, y));
                    displayCanvas.setPixel(x, y, Color::blend(top, bottom, 0.25f).pack());
                }
            }
        }
    }

    for (std::size_t y = 0; y < composite.getHeight(); ++y) {
        for (std::size_t x = 0; x < composite.getWidth(); ++x) {
            Color top = Color::unpack(composite.getPixel(x, y));
            Color bottom = Color::unpack(displayCanvas.getPixel(x, y));
            displayCanvas.setPixel(x, y, Color::blend(top, bottom, 1.0f).pack());
        }
    }
    return displayCanvas;
}

void Editor::goToFrame(int index) {
    activeLayerIndex_ = 0;
    timeline_.goToFrame(index);
}

void Editor::selectLayer(int newIndex) {
    if (newIndex >= 0 && newIndex <= getLayerStack().getLayerIndexes()) {
        activeLayerIndex_ = newIndex;
    }
}

int Editor::getLayerCount() {
    return timeline_.getCurrentFrame().getLayerStack().getLayerIndexes()+1;
}

int Editor::getWidth() const {
    return timeline_.getWidth();
}

int Editor::getHeight() const {
    return timeline_.getHeight();
}

std::string Editor::getFrameId(int frameIndex) const {
    return timeline_.getCurrentFrame().getId();
}

std::string Editor::getLayerId(int frameIndex, int layerIndex) const {
    return timeline_.getFrame(frameIndex).getLayerStack().getLayer(layerIndex).getId();
}