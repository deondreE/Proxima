#include "UI/Button.hpp"
#include <iostream>

namespace UI {

Button::Button(TTF_Font* defaultFont, SDL_Color defaultColor)
    : font(defaultFont),
      text_color(defaultColor),
      label_texture(nullptr),
      texture_needs_update(true),
      is_pressed(false) {}

Button::~Button() {
  if (label_texture) {
    SDL_DestroyTexture(label_texture);
    label_texture = nullptr;
  }
}

void Button::updateLabelTexture(SDL_Renderer* renderer) {
  if (!renderer || !font || label.empty()) {
    if (label_texture) {
      SDL_DestroyTexture(label_texture);
      label_texture = nullptr;
    }
    texture_needs_update = false;
    return;
  }

  if (!texture_needs_update) {
    return;
  }

  if (label_texture) {
    SDL_DestroyTexture(label_texture);
    label_texture = nullptr;
  }

  SDL_Surface* text_surface =
      TTF_RenderText_Blended(font, label.c_str(), label.length(), text_color);
  if (!text_surface) {
    std::cerr << "Button::updateLabelTexture: Failed to render text surface: "
              << std::endl;
    texture_needs_update = false;
    return;
  }

  label_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  if (!label_texture) {
    std::cerr
        << "Button::updateLabelTexture: Failed to create texture from surface: "
        << SDL_GetError() << std::endl;
    SDL_DestroySurface(text_surface);
    texture_needs_update = false;
    return;
  }

  SDL_DestroySurface(text_surface);
  texture_needs_update = false;
}

Button& Button::text(const std::string& str) {
  label = str;
  return *this;
}

Button& Button::onClick(std::function<void()> cb) {
  onClickHandler = cb;
  return *this;
}

Button& Button::setFont(TTF_Font* newFont) {
  if (font != newFont) {
    font = newFont;
    texture_needs_update = true;
  }
  return *this;
}

Button& Button::setColor(SDL_Color newColor) {
  if (text_color.r != newColor.r || text_color.g != newColor.g ||
      text_color.b != newColor.b || text_color.a != newColor.a) {
    text_color = newColor;
    texture_needs_update = true;
  }
  return *this;
}

void Button::click() {
  if (onClickHandler) {
    onClickHandler();
  }
}

void Button::draw(SDL_Renderer* renderer) {
  updateLabelTexture(renderer); 

  // bg
  if (is_pressed) {
    SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
  } else {
    SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0xDD, 0xFF);
  }
  
  SDL_FRect bg_rect = {(float)x, (float)y, (float)width, (float)height};
  SDL_RenderFillRect(renderer, &bg_rect);

  // border
  SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
  SDL_RenderRect(renderer, &bg_rect);

  if (label_texture) {
    float text_w, text_h;
    SDL_GetTextureSize(label_texture, &text_w, &text_h);
     SDL_FRect text_rect = { (float)x + (width - text_w) / 2.0f,
                                (float)y + (height - text_h) / 2.0f,
                                (float)text_w, (float)text_h };
    SDL_RenderTexture(renderer, label_texture, NULL, &text_rect);
  }

  View::draw(renderer); 
}

void Button::handleEvent(const SDL_Event& event) {
  if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    if (event.button.x >= x && event.button.x < (x + width) &&
        event.button.y >= y && event.button.y < (y + height)) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        is_pressed = true;
        click();
      }
    }
  }
  else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      is_pressed = false;
    }
  }
}

}  // Namespace UI
