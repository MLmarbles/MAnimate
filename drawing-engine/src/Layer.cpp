#include "Layer.hpp"
#include "History.hpp"

Layer::Layer(int width, int height) 
    : canvas_(width, height) {}

void Layer::setOpacity(float opacity) {
    if (opacity_ >= 1.0f || opacity_ <= 0) {
        return;
    }

    opacity_ += opacity;
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