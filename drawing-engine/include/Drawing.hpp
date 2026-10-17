#pragma once

#include "Canvas.hpp"
#include <cstdint>

void drawLine(
    Canvas& canvas,
    int x0,
    int y0,
    int x1,
    int y1,
    std::uint32_t color
);

void drawFilledCircle(
    Canvas& canvas,
    int centerX,
    int centerY,
    int radius,
    std::uint32_t color
);

void drawBrushStroke(
    Canvas& canvas,
    int x0,
    int y0,
    int x1,
    int y1,
    int radius,
    std::uint32_t color
);