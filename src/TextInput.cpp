#include "UI/TextInput.hpp"
#include <algorithm>
#include <iostream>

namespace UI {
TextInput::TextInput() {
  texture_needs_update = true;
}

TextInput::~TextInput() {}

void TextInput::setContext(const ViewContext& context) {
  View::setContext(context);

  if (!font && context.renderer) {
    font = _context.renderer->getTextRenderer()->loadFont(font_path, font_size);
    if (!font) {
      std::cerr << "ERROR: Failed to load font '" << font_path << "' for Text object (content: '" << textBuffer << "'). Falling back to Arial.\n";
      font = _context.renderer->getTextRenderer()->loadFont("Arial", font_size);
      if (!font) {
            std::cerr << "CRITICAL ERROR: Failed to load fallback 'Arial' font for Text object.\n";
      }
    }
  }
}

TextInput& TextInput::text(const std::string& str) {
  textBuffer = str;
  cursorPosition = textBuffer.length();
  return *this;
}

TextInput& TextInput::setFont(const std::string& fontPath, int size) {
  if (font_path != fontPath || font_size != size) {
    font_path = fontPath;
    font_size = size;
        // reopenFont();
  }
  return *this;
}

TextInput& TextInput::setColor(const Color& clr) {
  textColor = clr;
  return *this;
}

TextInput& TextInput::setCursorColor(const Color& clr) {
  cursorColor = clr;
  return *this;
}

TextInput& TextInput::setBackground(const Color& clr) {
  backgroundColor = clr;
  return *this;
}

void TextInput::draw(const ViewContext& context) {
  // bg
  UI::Renderer* renderer = context.renderer;
  auto textRenderer = context.renderer->getTextRenderer();

  renderer->setDrawColor(backgroundColor);
  renderer->fillRect(x, y, width, height);

  if (isFocused) {
    int cursorDrawX = 0;
    int byte_index_at_cursor = 0;
    std::string prefix_text;
    if (cursorPosition > 0 && !textBuffer.empty()) {
      const char* utf_text = textBuffer.c_str();
      for (int i = 0; utf_text[i] != '\0' && cursorDrawX < cursorPosition; ++i) {
        if ((utf_text[i] & 0xC0) !=  0x80) {
          cursorDrawX++;
        }
        byte_index_at_cursor = i+1;
      }

      if (cursorDrawX < cursorPosition) {
        byte_index_at_cursor = textBuffer.length();
      }
      prefix_text = textBuffer.substr(0, byte_index_at_cursor);
    } else {
      prefix_text = "";
    }
    
    auto size = textRenderer->measureText(textBuffer, font);
    int drawTextY = y + (height - font_size) / 2.0;
    cursorDrawX = size.first;

    textRenderer->drawText(textBuffer, font, textColor, width, drawTextY, x, y, false);

    renderer->setDrawColor(cursorColor);
    renderer->fillRect(x + 3.0 + cursorDrawX, y + (height - font_size) / 2.0, 2.0, font_size);
  }

  View::draw(context);
}

void TextInput::requestRedraw() {
  dirty = true;
  texture_needs_update = true;
}

bool TextInput::handleProximaEvent(const ProximaEvent& event) {
  bool consumed = false;
  int absX = x;
  int absY = y;

  switch (event.type) {
    case MOUSE_PRESS:
      if (event.x >= absX && event.x < (absX + width) && event.y >= absY &&
          event.y < (absY + height)) {
        if (!isFocused)
          setFocus(true);
        consumed = true;
      } else {
        if (isFocused) {
          setFocus(false);
          consumed = true;
        }
      }
      break;
    case KEY_PRESS: {
      if (isFocused) {
        if (event.keyCode == KEY_BACKSPACE) {
          deleteCharacter();
          dirty = true;
          consumed = true;
        } 
      }
      }
      break;
    case TEXT_INPUT:
      if (isFocused) {
        insertCharacter(event.text);
        dirty = true;
        consumed = true;
      }
      break;
    case WINDOW_RESIZE:
      dirty = true;
      break;

    default:
      break;
  }

  if (consumed)
    return true;

  return View::handleProximaEvent(event);
}


void TextInput::insertCharacter(const std::string& text_utf8) {
  if (!isFocused) return;
  if (cursorPosition >= 0 && cursorPosition <= (int)textBuffer.length()) {
    textBuffer.insert(cursorPosition, text_utf8);
    cursorPosition += text_utf8.length();
    texture_needs_update = true;
  }
}

void TextInput::deleteCharacter() {
  if (!isFocused) return;
  if (cursorPosition > 0) {
    textBuffer.erase(cursorPosition - 1, 1);
    cursorPosition--;
    texture_needs_update = true;
  }
}

void TextInput::moveCursor(int delta) {
  if (!isFocused) return;
  cursorPosition += std::clamp(cursorPosition + delta, 0, (int)textBuffer.length());
  texture_needs_update = true; 
}

}  // Namespace UI