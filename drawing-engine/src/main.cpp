#include "Canvas.hpp"
#include "Color.hpp"
#include "Drawing.hpp"
#include "ImageWriter.hpp"

#include <iostream>

int main()
{
    Canvas canvas(256, 256);

    Color white{255, 255, 255, 255};
    Color blue{0, 100, 255, 255};

    canvas.clear(white.pack());

    drawBrushStroke(
        canvas,
        30, 40,
        220, 190,
        10,
        blue.pack()
    );

    savePPM(canvas, "brush.ppm");

    std::cout << "Saved brush.ppm\n";

    return 0;
}