#include "Layer.hpp"
#include "History.hpp"
#include "UUID.h"
#include <algorithm>
#include <utility>

Layer::Layer(int width, int height) 
    : Layer(width, height, UUID::generate()) {}

Layer::Layer(int width, int height, std::string id)
    : id_(std::move(id)), canvas_(width, height) {}

const std::string& Layer::getId() const {
    return id_;
}

Layer Layer::duplicate() const {
    Layer duplicate = *this;
    duplicate.id_ = UUID::generate();
    return duplicate;
}

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
