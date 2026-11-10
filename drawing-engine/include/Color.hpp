#pragma once

#include <cstdint>
#include <algorithm>

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

    static Color blend(Color top, Color bottom, float layerOpacity) {
        layerOpacity = std::clamp(layerOpacity, 0.0f, 1.0f);
        float topAlpha = (top.a / 255.0f) * layerOpacity;
        float bottomAlpha = bottom.a / 255.0f;
        float outAlpha = topAlpha + bottomAlpha * (1.0f - topAlpha);

        if (outAlpha <= 0.0f) {
            return {0, 0, 0, 0};
        }

        Color result{};

        float red = (
            top.r * topAlpha +
            bottom.r * bottomAlpha * (1.0f - topAlpha)
        ) / outAlpha;

        float green = (
            top.g * topAlpha +
            bottom.g * bottomAlpha * (1.0f - topAlpha)
        ) / outAlpha;

        float blue = (
            top.b * topAlpha +
            bottom.b * bottomAlpha * (1.0f - topAlpha)
        ) / outAlpha;
        
        result.r = static_cast<std::uint8_t>(red);
        result.g = static_cast<std::uint8_t>(green);
        result.b = static_cast<std::uint8_t>(blue);
        result.a = static_cast<std::uint8_t>(outAlpha * 255.0f);

        return result;
    }
};