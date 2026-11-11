#include "Canvas.hpp"
#pragma once
#include "History.hpp"

class Layer{
    private:
        Canvas canvas_;
        float opacity_ = 1.0f;
        bool visible_ = true;
        History history_;

    public:
        Layer(int width, int height);

        void setOpacity(float opacity);
        void toggleVisible();
        float getOpacity() const;
        bool isVisible() const;
        Canvas& getCanvas();
        const Canvas& getCanvas() const;
        History& getHistory();
};