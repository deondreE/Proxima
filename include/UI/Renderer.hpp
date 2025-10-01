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

namespace UI {

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual void setDrawColor(const Color& color) = 0;
  virtual void drawRect(int x, int y, int w, int h) = 0;
  virtual void fillRect(int x, int y, int w, int h) = 0;
  virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

  virtual void present() = 0;
  virtual void clear() = 0;
};

} // Namespace UI