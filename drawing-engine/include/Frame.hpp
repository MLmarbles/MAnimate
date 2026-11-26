#include "LayerStack.hpp"
#include <string>
#include "InitializationMode.hpp"
#pragma once

class Frame{
    private:
        std::string id_;
        LayerStack layerStack_;
        
    public:
        Frame(int width, int height,
            InitializationMode mode = InitializationMode::Default);

        Frame(int width, int height, std::string id,
            InitializationMode mode = InitializationMode::Default);
            
        const std::string& getId() const;
        LayerStack& getLayerStack();
        const LayerStack& getLayerStack() const;
};
