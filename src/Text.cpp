#include "UI/Text.hpp"
#include <iostream>
#include <cassert>

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

void Text::setContext(const ViewContext& context) {
  View::setContext(context);

  if (!font && context.renderer) {
    font = _context.renderer->getTextRenderer()->loadFont(font_path, font_size);
    if (!font) {
      std::cerr << "ERROR: Failed to load font '" << font_path << "' for Text object (content: '" << text_content << "'). Falling back to Arial.\n";
      font = _context.renderer->getTextRenderer()->loadFont("Arial", font_size);
      if (!font) {
            std::cerr << "CRITICAL ERROR: Failed to load fallback 'Arial' font for Text object.\n";
      }
    }
  }
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

void Text::draw(const ViewContext& context) {
  UI::Renderer* renderer = context.renderer;
  assert(renderer && "Renderer must be valid in ViewContext for drawing!");
  assert(_context.renderer == renderer && "Context mismatch in Text::draw!");

  auto textRenderer = renderer->getTextRenderer();
  if (!font) {
    std::cerr << "WARNING: Font not available for Text object '" << text_content << "', skipping draw." << std::endl;
    View::draw(context);
    return;
  }

    auto textDim = textRenderer->measureText(text_content, font);
    textRenderer->drawText(text_content, font, text_color, textDim.first, textDim.second, width*2, height, false);

    View::draw(context);
}

bool Text::handleProximaEvent(const ProximaEvent& event) {
  return View::handleProximaEvent(event);
}

}  // Namespace UI