#include "Drawing.hpp"

#include <algorithm>
#include <cmath>

void drawLine(
    Canvas& canvas,
    int x0,
    int y0,
    int x1,
    int y1,
    std::uint32_t color
)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = std::max(std::abs(dx), std::abs(dy));

    if (steps == 0) {
        canvas.setPixel(x0, y0, color);
        return;
    }

    double xIncrement = static_cast<double>(dx) / steps;
    double yIncrement = static_cast<double>(dy) / steps;

    double x = static_cast<double>(x0);
    double y = static_cast<double>(y0);

    for (int i = 0; i <= steps; ++i) {

        canvas.setPixel(std::round(x), std::round(y), color);
        x += xIncrement;    
        y += yIncrement;
    }
}



void drawFilledCircle(
    Canvas& canvas,
    int centerX,
    int centerY,
    int radius,
    std::uint32_t color
)
{

    for (int y = centerY-radius; y <= centerY+radius; ++y) {
        for (int x = centerX-radius; x <= centerX+radius; ++x) {

            int dx = x - centerX;
            int dy = y - centerY;

            if (sqrt(dy * dy + dx * dx) <= radius) {
                canvas.setPixel(x, y, color);
            }
        }
    }

}
