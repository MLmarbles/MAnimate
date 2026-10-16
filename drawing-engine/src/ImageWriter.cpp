#include "ImageWriter.hpp"
#include "Color.hpp"
#include <iostream>

#include <fstream>
#include <stdexcept>

void savePPM(const Canvas& canvas, const std::string& filename)
{
    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("Could not open image file");
    }

    file << "P3" << "\n";
    file << canvas.getWidth() << " ";
    file << canvas.getHeight() << "\n";
    file << "255\n";

    for (std::size_t i = 0; i < canvas.getHeight(); ++i) {
        for (std::size_t j = 0; j < canvas.getWidth(); ++j) {
            std::uint32_t pixel = canvas.getPixel(j, i);
            Color result = Color::unpack(pixel);

            file << static_cast<int>(result.r) << " ";
            file << static_cast<int>(result.g) << " ";
            file << static_cast<int>(result.b) << " ";
        }
     file << "\n";
    }
}


