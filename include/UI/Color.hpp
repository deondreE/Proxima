#pragma once
#include <string>
#include <tuple>

struct Color {
  uint8_t r, g, b, a;

  constexpr Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255)
      : r(r), g(g), b(b), a(a) {}

  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Transparent;
};

typedef Color color_t;

inline const Color Color::Black = Color(0, 0, 0);
inline const Color Color::White = Color(255, 255, 255);
inline const Color Color::Red = Color(255, 0, 0);
inline const Color Color::Green = Color(0, 255, 0);
inline const Color Color::Blue = Color(0, 0, 255);
inline const Color Color::Transparent = Color(0, 0, 0, 0);