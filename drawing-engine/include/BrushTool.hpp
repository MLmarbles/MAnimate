#pragma once

#include "Canvas.hpp"
#include "Tool.hpp"

class BrushTool : public Tool {
    private:
        bool drawing = false;
        int prevX = 0;
        int prevY = 0;
        int radius = 5;
        std::uint32_t color;

    public:
        BrushTool();

        void onMouseDown(Canvas& canvas, int x, int y) override;
        void onMouseMove(Canvas& canvas, int x, int y) override;
        void onMouseUp(Canvas& canvas, int x, int y) override;

};