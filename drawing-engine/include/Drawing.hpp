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