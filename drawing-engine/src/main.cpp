#include "Canvas.hpp"
#include <iostream>
#include <Color.hpp>
#include "ImageWriter.hpp"

int main()
{
    Canvas canvas(3, 2);

    Color red{255, 0, 0, 255};
    Color green{0, 255, 0, 255};
    Color blue{0, 0, 255, 255};

    Color white{255, 255, 255, 255};
    Color yellow{255, 255, 0, 255};
    Color black{0, 0, 0, 255};

    canvas.setPixel(0, 0, red.pack());
    canvas.setPixel(1, 0, green.pack());
    canvas.setPixel(2, 0, blue.pack());

    canvas.setPixel(0, 1, white.pack());
    canvas.setPixel(1, 1, yellow.pack());
    canvas.setPixel(2, 1, black.pack());

    savePPM(canvas, "output.ppm");

    std::cout << "saved output.ppm\n";

    return 0;
}