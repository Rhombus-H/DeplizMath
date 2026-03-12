#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

struct Color {
    uint8_t r, g, b, a;
    constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}
};

// цвета для графиков (8 штук)
class ColorPalette {
public:
    static constexpr std::array<Color, 8> COLORS = {{
        {0x1F, 0x77, 0xB4, 255},  // синий
        {0xFF, 0x7F, 0x0E, 255},  // оранжевый
        {0x2C, 0xA0, 0x2C, 255},  // зеленый
        {0xD6, 0x27, 0x28, 255},  // красный
        {0x94, 0x67, 0xBD, 255},  // фиолетовый
        {0x8C, 0x56, 0x4B, 255},  // коричневый
        {0xE3, 0x77, 0xC2, 255},  // розовый
        {0x7F, 0x7F, 0x7F, 255},  // серый
    }};

    static constexpr Color getColor(std::size_t index) {
        return COLORS[index % COLORS.size()];
    }

    // то же но с другой прозрачностью
    static constexpr Color withAlpha(std::size_t index, uint8_t alpha) {
        Color c = getColor(index);
        return Color(c.r, c.g, c.b, alpha);
    }
};
