#include "UI/Text.hpp"
#include <iostream>

namespace UI {

Text::Text(const std::string& text, const std::string& initial_font_path,
          int initial_font_size, const Color& color)
    : text_content(text),
      font(nullptr),                 // Initialize font to nullptr
      font_path(initial_font_path),  // Store path
      font_size(initial_font_size),  // Store size
      text_color(color),
      word_wrap(false) {
}

Text::~Text() {
}
Text& Text::content(const std::string& str) {
  if (text_content != str) {
    text_content = str;
  }
  return *this;
}

Text& Text::setFont(const std::string& fontPath, int size) {
  if (font_path != fontPath || font_size != size) {
    font_path = fontPath;
    font_size = size;
  }
  return *this;
}

Text& Text::setColor(const Color& newColor) {
  if (text_color.r != newColor.r || text_color.g != newColor.g ||
      text_color.b != newColor.b || text_color.a != newColor.a) {
    text_color = newColor;
  }
  return *this;
}

Text& Text::setWordWrap(bool wrap) {
  if (word_wrap != wrap) {
    word_wrap = wrap;
  }
  return *this;
}

void Text::draw(Renderer* renderer) {
  auto& textRenderer = renderer->getTextRenderer();
  // Load font, check for updates.
  font = textRenderer.loadFont(font_path, font_size);
  auto size = textRenderer.measureText(text_content, font);
  textRenderer.drawText(text_content, font, text_color, 100, 100, size.first, size.second, false);
}

bool Text::handleProximaEvent(const ProximaEvent& event) {
  return View::handleProximaEvent(event);
}

}  // Namespace UI