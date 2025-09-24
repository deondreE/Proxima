#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <string>
#include "View.hpp"
#include "Core/ProximaEvent.hpp"

namespace UI {

class Button : public View {
  std::string label;
  std::function<void()> onClickHandler;
  TTF_Font* font;
  std::string font_path;
  int font_size;
  SDL_Color text_color;
  SDL_Texture* label_texture;
  bool texture_needs_update;
  bool is_pressed;

  void updateLabelTexture(SDL_Renderer* renderer);
  void reopenFont();

 public:
  Button(const std::string& text = "",
         const std::string& initial_font_path = "", int initial_font_size = 24,
         SDL_Color defaultColor = {0, 0, 0, 255});
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
  Button& onClick(std::function<void()> cb);
  Button& setFont(const std::string& fontPath, int size);
  Button& setColor(SDL_Color newColor);

  void click();
  void draw(SDL_Renderer* renderer) override;
  bool handleProximaEvent(const ProximaEvent& event) override;

  Button(const Button&) = delete;
  Button& operator=(const Button&) = delete;
  Button(Button&&) = delete;
  Button& operator=(Button&&) = delete;
};
}  // Namespace UI
