#include "Frame.hpp"
#include "LayerStack.hpp"
#include "Color.hpp"
#include "UUID.h"
#include <utility>
#include <string>

Frame::Frame(int width, int height)
    : Frame(width, height, UUID::generate())
{}

Frame::Frame(int width, int height, std::string id)
    : id_(std::move(id)), layerStack_(LayerStack(width, height))
{
    layerStack_.addLayer();

    layerStack_.getLayer(0).getCanvas().clear(
        Color{0, 0, 0, 0}.pack()
    );
}

const std::string& Frame::getId() const {
    return id_;
}

LayerStack& Frame::getLayerStack() {
    return layerStack_;
}

const LayerStack& Frame::getLayerStack() const {
    return layerStack_;
}
