#include "LayerStack.hpp"
#pragma once

class Frame{
    private:
        LayerStack layerStack_;

    public:
        Frame(int width, int height);
        LayerStack& getLayerStack();
};