#include "Layer.hpp"

Layer::Layer(int width, int height) 
    : canvas_(width, height) {}

void Layer::setOpacity(float opacity) {
    opacity_ = opacity;
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