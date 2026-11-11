#include "LayerStack.hpp"
#include "Layer.hpp"
#include <Color.hpp>

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