#pragma once

#include "Tool.hpp"

class EraserTool : public Tool {
    private:
        bool erasing = false;
        int prevX = 0;
        int prevY = 0;
        int radius = 5;

    public:
        EraserTool();

        void onMouseDown(Canvas& canvas, int x, int y) override;
        void onMouseMove(Canvas& canvas, int x, int y) override;
        void onMouseUp(Canvas& canvas, int x, int y) override;
};