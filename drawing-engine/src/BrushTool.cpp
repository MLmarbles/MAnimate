#include "BrushTool.hpp"
#include "Color.hpp"
#include "Drawing.hpp"

BrushTool::BrushTool() {
    Color red{255, 0, 0, 255};
    color = red.pack();
}

void BrushTool::onMouseDown(Canvas& canvas, int x, int y) {
    drawing = true;
    prevX = x;
    prevY = y; 

    drawBrushStroke(
        canvas,
        prevX,
        prevY,
        x,
        y,
        radius,
        color
    );
}

void BrushTool::onMouseMove(Canvas& canvas, int x, int y) {
    if (drawing) {
        drawBrushStroke(
            canvas,
            prevX,
            prevY,
            x,
            y,
            radius,
            color
        );

        prevX = x;
        prevY = y;
    }
}

void BrushTool::onMouseUp(Canvas& canvas, int x, int y){
    drawing = false;

    drawBrushStroke(
        canvas,
        prevX,
        prevY,
        x,
        y,
        radius,
        color
    );
}