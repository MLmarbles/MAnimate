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

    drawFilledCircle(
        canvas,
        128, 128,
        40,
        blue.pack()
    );

    savePPM(canvas, "circle.ppm");

    std::cout << "Saved circle.ppm\n";

    return 0;
}