#pragma once
#include <cstdint>
#include <algorithm>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() : r(0), g(0), b(0), a(255) {}
    
    Color(int red, int green, int blue, int alpha = 255)
        : r(static_cast<uint8_t>(std::clamp(red, 0, 255))),
          g(static_cast<uint8_t>(std::clamp(green, 0, 255))),
          b(static_cast<uint8_t>(std::clamp(blue, 0, 255))),
          a(static_cast<uint8_t>(std::clamp(alpha, 0, 255))) {}

    Color operator+(const Color& other) const {
        return Color(
            std::min(255, r + other.r),
            std::min(255, g + other.g),
            std::min(255, b + other.b),
            a
        );
    }

    Color operator*(float factor) const {
        return Color(
            static_cast<uint8_t>(std::clamp(r * factor, 0.0f, 255.0f)),
            static_cast<uint8_t>(std::clamp(g * factor, 0.0f, 255.0f)),
            static_cast<uint8_t>(std::clamp(b * factor, 0.0f, 255.0f)),
            a
        );
    }

    friend Color operator*(float factor, const Color& color) {
        return color * factor;
    }
};