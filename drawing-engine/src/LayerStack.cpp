#include "LayerStack.hpp"
#include "Layer.hpp"
#include <Color.hpp>
#include <algorithm>
#include <stdexcept>

LayerStack::LayerStack(int width, int height)
    : width_(width), height_(height) {}

void LayerStack::addLayer() {
    layers_.emplace_back(width_, height_);
}

Layer& LayerStack::getLayer(int index) {
    return layers_.at(index);
}

Canvas LayerStack::composite() const {
    Canvas result(width_, height_);

    for (const Layer& layer : layers_) {
        if (!layer.isVisible()) {
            continue;
        }

        for (std::size_t y = 0; y < height_; ++y) {
            for (std::size_t x = 0; x < width_; ++x) {
                Color top = Color::unpack(
                    layer.getCanvas().getPixel(x, y)
                );

                Color bottom = Color::unpack(
                    result.getPixel(x, y)
                );

                Color blended = Color::blend(
                    top,
                    bottom,
                    layer.getOpacity()
                );

                result.setPixel(x, y, blended.pack());
            }
        }
    }

    return result;
}

void LayerStack::moveLayer(int fromIndex, int toIndex) {
    int count = static_cast<int>(layers_.size());

    if (fromIndex < 0 || fromIndex >= count ||
        toIndex < 0 || toIndex >= count) {
        throw std::out_of_range("Invalid layer index");
    }

    if (fromIndex == toIndex) {
        return;
    }

    if (fromIndex < toIndex) {
        std::rotate(
            layers_.begin() + fromIndex,
            layers_.begin() + fromIndex + 1,
            layers_.begin() + toIndex + 1
        );
    } else {
        std::rotate(
            layers_.begin() + toIndex,
            layers_.begin() + fromIndex,
            layers_.begin() + fromIndex + 1
        );
    }
}

void LayerStack::removeLayer(int index) {
    if (index > layers_.size()-1) {
        throw std::out_of_range("Invalid layer index");
    }

    layers_.erase(layers_.begin() + index);
}

int LayerStack::getLayerIndexes() {
    return layers_.size()-1;
}