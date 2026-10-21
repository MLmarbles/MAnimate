#pragma once

#include "Canvas.hpp"

class Tool {
    private:

    public:
        virtual void onMouseDown(Canvas& canvas, int x, int y) = 0;
        virtual void onMouseMove(Canvas& canvas, int x, int y) = 0;
        virtual void onMouseUp(Canvas& canvas, int x, int y) = 0;

        virtual ~Tool() = default;
};