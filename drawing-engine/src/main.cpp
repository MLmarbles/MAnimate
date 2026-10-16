#include "Canvas.hpp"
#include "Color.hpp"
#include "Drawing.hpp"
#include "ImageWriter.hpp"

#include <iostream>

int main()
{
    Canvas canvas(256, 256);

    Color white{255, 255, 255, 255};
    Color red{255, 0, 0, 255};

    canvas.clear(white.pack());

    drawLine(
        canvas,
        20, 20,
        230, 200,
        red.pack()
    );

    savePPM(canvas, "line.ppm");

    std::cout << "saved line.ppm\n";

    return 0;
}