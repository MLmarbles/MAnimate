#include "Canvas.hpp"

Canvas::Canvas(std::size_t width, std::size_t height)
        : width_{width},
        height_{height},
        pixels_(width_ * height_)
    {}

std::size_t Canvas::getWidth() const{
    return width_;
}

std::size_t Canvas::getHeight() const{
    return height_;
}

void Canvas::setPixel(std::size_t x, std::size_t y, std::uint32_t color) {
    pixels_[(width_ * y) + x] = color;
}

std::uint32_t Canvas::getPixel(std::size_t x, std::size_t y) const {
    return pixels_[(width_ * y) + x];
}