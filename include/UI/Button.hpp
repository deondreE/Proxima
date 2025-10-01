#pragma once

#if defined (_WIN32) 
  #include "Core/WinTextRenderer.hpp"
#elif defined(__linux__) 
#include <SDL3_ttf/SDL_ttf.h>
#endif

#include <functional>
#include <string>
#include "View.hpp"
#include "Color.hpp"
#include "Core/ProximaEvent.hpp"
#include "Core/TextRenderer.hpp"
#include "Platform.hpp"

namespace UI {

class PEXPORT Button : public View {
  std::string label;
  std::function<void(Button&)> onClickHandler;
  Core::Font* font;
  std::string font_path;
  int font_size;
  Color text_color;
  bool is_pressed;

 public:
  Button(const std::string& text = "",
        const std::string& initial_font_path = "", int initial_font_size = 24,
        const Color& defaultTextColor = {0, 0, 0, 255});
  ~Button() override;

  Button& size(int w, int h) {
    View::size(w, h);
    return *this;
  }
  Button& pos(int nx, int ny) {
    View::pos(nx, ny);
    return *this;
  }
  Button& text(const std::string& str);
  Button& onClick(std::function<void(Button&)> cb);
  Button& setFont(const std::string& fontPath, int size);
  Button& setTextColor(const Color& newColor);
  void setContext(const ViewContext& context) override;

  void click();
  void draw(const ViewContext& context) override;
  bool handleProximaEvent(const ProximaEvent& event) override;

  Button(const Button&) = delete;
  Button& operator=(const Button&) = delete;
  Button(Button&&) = delete;
  Button& operator=(Button&&) = delete;
};
}  // Namespace UI
