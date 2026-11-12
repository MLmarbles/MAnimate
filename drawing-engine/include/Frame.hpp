#include "LayerStack.hpp"

class Frame{
    private:
        LayerStack layerStack_;

    public:
        Frame(int width, int height);
        LayerStack& getLayerStack();
};