#include "LayerStack.hpp"
#include <string>
#pragma once

class Frame{
    private:
        std::string id_;
        LayerStack layerStack_;
        
    public:
        Frame(int width, int height);
        Frame(int width, int height, std::string id);
        const std::string& getId() const;
        LayerStack& getLayerStack();
        const LayerStack& getLayerStack() const;
};
