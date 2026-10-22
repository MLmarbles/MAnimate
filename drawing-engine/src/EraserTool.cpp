#include "EraserTool.hpp"
#include "Color.hpp"
#include "Drawing.hpp"

EraserTool::EraserTool() {}

void EraserTool::onMouseDown(Canvas& canvas, int x, int y) {
    erasing = true;
    prevX = x;
    prevY = y; 
    Color white{255, 255, 255, 255};

    drawBrushStroke(
        canvas,
        prevX,
        prevY,
        x,
        y,
        radius,
        white.pack()
    );
}

void EraserTool::onMouseMove(Canvas& canvas, int x, int y) {
    Color white{255, 255, 255, 255};
    if (erasing) {
        drawBrushStroke(
            canvas,
            prevX,
            prevY,
            x,
            y,
            radius,
            white.pack()
        );

        prevX = x;
        prevY = y;
    }
}

void EraserTool::onMouseUp(Canvas& canvas, int x, int y){
    erasing = false;
    Color white{255, 255, 255, 255};

    drawBrushStroke(
        canvas,
        prevX,
        prevY,
        x,
        y,
        radius,
        white.pack()
    );
}