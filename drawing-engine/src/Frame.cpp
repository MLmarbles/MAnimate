#include "Frame.hpp"
#include "LayerStack.hpp"
#include "Color.hpp"

Frame::Frame(int width, int height)
    : layerStack_(LayerStack(width, height)) 
{
    layerStack_.addLayer();

    layerStack_.getLayer(0).getCanvas().clear(
        Color{0, 0, 0, 0}.pack()
    );
}

LayerStack& Frame::getLayerStack() {
    return layerStack_;
}
