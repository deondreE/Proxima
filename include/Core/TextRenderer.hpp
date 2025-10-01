#pragma once

#include <string>
#include "UI/Color.hpp"

namespace Core {
  class Font {
    public:
    virtual ~Font() = default;
    virtual int getSize() const = 0;
  };

  class TextRenderer { 
    public:
      virtual ~TextRenderer() = default;

      virtual Font* loadFont(const std::string& file, int pSize) = 0;
      virtual void drawText(const std::string& text, 
                            Font* font,
                            const Color& color,
                            int maxWidth, int maxHeight,
                            int x, int y,
                            bool wordWrap = false) = 0;
      virtual std::pair<int, int> measureText(const std::string& text, Font* font) = 0;
      virtual std::pair<int, int> measureWrappedText(const std::string& text, Font* font, int maxWidth) = 0;
  };
} // Core