#include "Layer.hpp"
#include "History.hpp"
#include <algorithm>

Layer::Layer(int width, int height) 
    : canvas_(width, height) {}

void Layer::setOpacity(float opacity) {
    opacity_ = std::clamp(opacity_ + opacity, 0.0f, 1.0f);
}

void Layer::toggleVisible() {
    visible_ = !visible_;
}

float Layer::getOpacity() const {
    return opacity_;
}

bool Layer::isVisible() const {
    return visible_;
}

Canvas& Layer::getCanvas() {
    return canvas_;
}

const Canvas& Layer::getCanvas() const {
    return canvas_;
}

History& Layer::getHistory() {
    return history_;
}