#include "UI/TextInput.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace UI {
TextInput::TextInput() {
  texture_needs_update = true;
}

void TextInput::printHexString(
    const std::string& str,
    int bytesPerLine,
    bool showAscii
) {
    if (str.empty()) {
        std::cout << "(empty string)" << std::endl;
        return;
    }

    std::cout << "String (length " << str.length() << " bytes) in hex:"
              << std::endl;

    std::ios_base::fmtflags originalFlags = std::cout.flags();

    std::cout << std::hex << std::setfill('0');

    for (size_t i = 0; i < str.length(); ++i) {
        std::cout << std::setw(2)
                  << static_cast<unsigned int>(
                        static_cast<unsigned char>(str[i])
                    )
                  << " ";

        if ((i + 1) % bytesPerLine == 0 || (i + 1) == str.length()) {
            if (showAscii) {
                if ((i + 1) % bytesPerLine != 0 && (i + 1) == str.length()) {
                    int remainingBytes = bytesPerLine - ((i + 1) % bytesPerLine);
                    if (remainingBytes == bytesPerLine) {
                        remainingBytes = 0;
                    }
                    for (int j = 0; j < remainingBytes; ++j) {
                        std::cout << "   ";
                    }
                }

                std::cout << " | ";
                size_t startOfLine = (i / bytesPerLine) * bytesPerLine;
                for (size_t j = startOfLine; j <= i; ++j) {
                    char c = str[j];
                    if (c >= 32 && c <= 126) {
                        std::cout << c;
                    } else {
                        std::cout << ".";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    std::cout.flags(originalFlags);
    std::cout << std::endl;
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

  int drawY = y + (height - font_size) / 2.0;
  if (!textBuffer.empty()) {
    textRenderer->drawText(textBuffer, font, textColor, x, drawY, width, height, false);
  }

  if (texture_needs_update) {
    textRenderer->drawText(textBuffer, font, textColor, x, drawY, width, height, false);
    texture_needs_update = false;
  }

  if (isFocused) {
    int cursorDrawX = 0;
    int byte_index_at_cursor = 0;
    std::string prefix_text;
    if (cursorPosition > 0 && !textBuffer.empty()) {
      const char* utf_text = textBuffer.c_str();
      for (int i = 0; utf_text[i] != '\0' && cursorDrawX < cursorPosition; ++i) {
        if (((unsigned char)utf_text[i] & 0xC0) !=  0x80) {
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
    
    auto size = textRenderer->measureText(prefix_text, font);
    cursorDrawX = size.first;

    renderer->setDrawColor(cursorColor);
    renderer->fillRect(x + 3.0 + cursorDrawX, drawY, 2.0, font_size);
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

  size_t byte_index = getByteIndexForCharIndex(textBuffer, cursorPosition);
  textBuffer.insert(byte_index, text_utf8);
  cursorPosition += getCharCount(text_utf8);
  texture_needs_update = true;
}

void TextInput::deleteCharacter() {
  if (!isFocused) return;
  if (cursorPosition == 0) return;

  size_t byte_end_of_char_to_delete = getByteIndexForCharIndex(textBuffer, cursorPosition);
  if (byte_end_of_char_to_delete == 0) return;
  size_t byte_start_of_char_to_delete = byte_end_of_char_to_delete - 1;
  // Move backward until we find a leading byte (or the start of the string)
  while (byte_start_of_char_to_delete > 0 &&
        ((unsigned char)textBuffer[byte_start_of_char_to_delete] & 0xC0) == 0x80) {
    byte_start_of_char_to_delete--;
  }
  size_t bytes_to_erase = byte_end_of_char_to_delete - byte_start_of_char_to_delete;

  printHexString(textBuffer);

  if (bytes_to_erase > 0) {
    textBuffer.erase(byte_start_of_char_to_delete, bytes_to_erase);
    printHexString(textBuffer);
    cursorPosition--;
    texture_needs_update = true;
  }
}

size_t TextInput::getByteIndexForCharIndex(const std::string& str, int char_index) {
  int current_char_count = 0;
  for (size_t i = 0; i < str.length(); ++i) {
    if (current_char_count == char_index) {
      return i;
    }
    if (((unsigned char)str[i] & 0xC0) != 0x80) {
      current_char_count++;
    }
  }
  return str.length(); 
}

int TextInput::getCharCount(const std::string& str) {
  int count = 0;
  for (size_t i = 0; i < str.length(); ++i) {
    if ((str[i] & 0xC0) != 0x80) {
      count++;
    }
  }
  return count;
}

void TextInput::moveCursor(int delta) {
  if (!isFocused) return;
  int current_char_count = getCharCount(textBuffer);
  cursorPosition = std::clamp(cursorPosition + delta, 0, current_char_count);
  texture_needs_update = true;
}

}  // Namespace UI