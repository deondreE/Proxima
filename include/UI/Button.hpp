#pragma once
#include <functional>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "View.hpp"

namespace UI {

class Button : public View {
  std::string label;
  std::function<void()> onClickHandler;
  TTF_Font* font;
  SDL_Color text_color;
  SDL_Texture* label_texture;
  bool texture_needs_update; 

  void updateLabelTexture(SDL_Renderer* renderer);

 public:
  Button(TTF_Font* defaultFont = nullptr, SDL_Color defaultColor = {0,0,0,255});
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
  Button& setFont(TTF_Font* newFont);
  Button& setColor(SDL_Color newColor);

  void click();
  void draw(SDL_Renderer* renderer) override;
  void handleEvent(const SDL_Event& event) override;

  Button(const Button&) = delete;
  Button& operator=(const Button&) = delete;
  Button(Button&&) = delete;
  Button& operator=(Button&&) = delete;
};
}  // Namespace UI
