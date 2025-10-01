#pragma once
#include <string>
#include "View.hpp"
#include "Color.hpp"
#include "Core/TextRenderer.hpp"
#include "Core/ProximaEvent.hpp"
#include "Platform.hpp"  

namespace UI {

class PEXPORT Text : public View {
 private:
  std::string text_content;
  Color text_color;
  Core::Font* font;
  std::string font_path;
  int font_size;

  bool word_wrap;

 public:
  Text(const std::string& text = "", const std::string& initial_font_path = "",
      int initial_font_size = 24, const Color& color = {0, 0, 0, 255});

  ~Text() override;

  Text& size(int w, int h) {
    View::size(w, h);
    return *this;
  }
  Text& pos(int nx, int ny) {
    View::pos(nx, ny);
    return *this;
  }
  Text& content(const std::string& str);
  Text& setWordWrap(bool wrap);
  Text& setFont(const std::string& fontPath, int size);
  Text& setColor(const Color& newColor);

  void draw(const ViewContext& context) override;
  void setContext(const ViewContext& context) override;
  bool handleProximaEvent(const ProximaEvent& event) override;

  Text(const Text&) = delete;
  Text& operator=(const Text&) = delete;
  Text(Text&&) = delete;
  Text& operator=(Text&&) = delete;
};

}  // Namespace UI