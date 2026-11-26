#include "UUID.h"

#include <array>
#include <cstdint>
#include <random>
#include <sstream>
#include <iomanip>

std::string UUID::generate() {
    static thread_local std::mt19937 generator{
        std::random_device{}()
    };

    std::uniform_int_distribution<int> distribution(0, 255);

    std::array<std::uint8_t, 16> bytes{};

    for (auto& byte : bytes) {
        byte = static_cast<std::uint8_t>(
            distribution(generator)
        );
    }

    bytes[6] = (bytes[6] & 0x0F) | 0x40;

    bytes[8] = (bytes[8] & 0x3F) | 0x80;

    std::ostringstream stream;

    stream << std::hex << std::setfill('0');

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            stream << '-';
        }

        stream << std::setw(2)
               << static_cast<int>(bytes[i]);
    }

    return stream.str();
