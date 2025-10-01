#pragma once

#include "Color.hpp"
#include "Core/TextRenderer.hpp"
#include <string>
#include <tuple>

namespace UI {

class Renderer {
 public:
  virtual ~Renderer() = default;

  virtual void setDrawColor(const Color& color) = 0;
  virtual void drawRect(int x, int y, int w, int h) = 0;
  virtual void fillRect(int x, int y, int w, int h) = 0;
  virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
  virtual Core::TextRenderer* getTextRenderer() const = 0;

  virtual void present() = 0;
  virtual void clear() = 0;
};

} // Namespace UI