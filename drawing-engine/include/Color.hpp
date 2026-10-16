#pragma once

#include <cstdint>

struct Color {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;

    std::uint32_t pack() const {
        std::uint32_t result = 0;

        result |= static_cast<std::uint32_t>(r) << 24;
        result |= static_cast<std::uint32_t>(g) << 16;
        result |= static_cast<std::uint32_t>(b) << 8;
        result |= static_cast<std::uint32_t>(a) << 0;

        return result;
    }

    static Color unpack(std::uint32_t packed)
    {
        Color result;

        result.r = static_cast<std::uint8_t>((packed >> 24) & 0xFF);
        result.g = static_cast<std::uint8_t>((packed >> 16) & 0xFF);
        result.b = static_cast<std::uint8_t>((packed >> 8) & 0xFF);
        result.a = static_cast<std::uint8_t>((packed >> 0) & 0xFF);

        return result;
    }
};