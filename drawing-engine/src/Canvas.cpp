#include "Canvas.hpp"
#include <stdexcept>
#include <Color.hpp>
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

const std::vector<std::uint32_t>& Canvas::getPixels() const {
    return pixels_;
}

std::vector<uint8_t> Canvas::exportRGBA() const {
    std::vector<uint8_t> data;
    data.reserve(pixels_.size() * 4);

    for (uint32_t pixel : pixels_) {
        Color color = Color::unpack(pixel);

        data.push_back(color.r);
        data.push_back(color.g);
        data.push_back(color.b);
        data.push_back(color.a);
    }

    return data;
}

void Canvas::importRGBA(const std::vector<uint8_t>& data) {
    if (data.size() != pixels_.size() * 4) {
        throw std::invalid_argument("size mismatch");
    }

    for (std::size_t i = 0; i < pixels_.size(); ++i) {
        std::size_t offset = i * 4;

        Color color{
            data[offset],
            data[offset + 1],
            data[offset + 2],
            data[offset + 3]
        };

        pixels_[i] = color.pack();
    }
}