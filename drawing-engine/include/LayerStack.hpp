#include "Layer.hpp"
#include <vector>
#pragma once

class LayerStack{
    private:
        int width_;
        int height_;
        std::vector<Layer> layers_;

    public:
        LayerStack(int width, int height);
        void addLayer();
        Layer& getLayer(int index);
        Canvas composite() const;
};