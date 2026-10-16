#include "Canvas.hpp"
#include <stdexcept>

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
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("outside bounds");
    }

    pixels_[(width_ * y) + x] = color;
}

std::uint32_t Canvas::getPixel(std::size_t x, std::size_t y) const {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("outside bounds");
    }
    
    return pixels_[(width_ * y) + x];
}

void Canvas::clear(std::uint32_t color) {
    for (auto& pixel : pixels_) {
        pixel = color;
    }
}