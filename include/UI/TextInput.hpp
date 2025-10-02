#pragma once
#include <string>
#include <vector>
#include "Core/ProximaEvent.hpp"
#include "Core/TextRenderer.hpp"
#include "View.hpp"
#include "Color.hpp"
#include "Platform.hpp"

namespace UI {

class PEXPORT TextInput : public View {
  std::string textBuffer;
  int cursorPosition = 0;
  bool isFocused = false;

  Core::Font* font = nullptr;
  std::string font_path;
  bool dirty = false;
  int font_size;

  bool texture_needs_update;
  Color textColor = {0, 0, 0, 255};
  Color cursorColor = {0, 0, 0, 255};
  Color backgroundColor = {255, 255, 255, 255};

  size_t getByteIndexForCharIndex(const std::string& str, int char_index); 
  int getCharCount(const std::string& str);
  void printHexString(
    const std::string& str,
    int bytesPerLine = 16,
    bool showAscii = true
);

 public:
  TextInput();
  ~TextInput() override;

  TextInput& text(const std::string& str);
  TextInput& setFont(const std::string& font_path, int font_size);
  TextInput& setColor(const Color& clr);
  TextInput& setCursorColor(const Color& clr);
  TextInput& setBackground(const Color& clr);

  void draw(const ViewContext& context) override;
  bool handleProximaEvent(const ProximaEvent& event) override;
  void setContext(const ViewContext& context) override;

  void setNeedsRedraw(bool val);
  void requestRedraw();

  void insertCharacter(const std::string& text_utf8);  // Use text input data
  void deleteCharacter();
  void moveCursor(int delta);
  inline void setFocus(bool focus) {
    isFocused = focus;
  }
  bool hasFocus() const { return isFocused; }
};

}  // Namespace UI