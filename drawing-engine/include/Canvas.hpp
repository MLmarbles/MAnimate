#pragma once
#include <vector>
#include <cstdint>

class Canvas {
    private:
        std::size_t width_;
        std::size_t height_;
        std::vector<std::uint32_t> pixels_;

    public:
        Canvas(std::size_t width, std::size_t height);

        std::size_t getWidth() const;
        std::size_t getHeight() const;

        void setPixel(std::size_t x, std::size_t y, std::uint32_t color);
        std::uint32_t getPixel(std::size_t x, std::size_t y) const;
        void clear(std::uint32_t color);
};