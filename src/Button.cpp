#include "UI/Button.hpp"
#include <iostream>

namespace UI {

Button::Button(const std::string& text, const std::string& initial_font_path,
              int initial_font_size, const Color& defaultTextColor)
    : label(text),
      onClickHandler([](Button&) {}), 
      font(nullptr),
      font_path(initial_font_path),
      font_size(initial_font_size),
      text_color(defaultTextColor),
      is_pressed(false) {
}

Button::~Button() {
}

Button& Button::text(const std::string& str) {
  label = str;
  return *this;
}

Button& Button::onClick(std::function<void(Button&)> cb) {
  onClickHandler = std::move(cb);
  return *this;
}

Button& Button::setFont(const std::string& fontPath, int size) {
  if (font_path != fontPath || font_size != size) {
    font_path = fontPath;
    font_size = size;
  }
  return *this;
}

Button& Button::setTextColor(const Color& newColor) {
  if (text_color.r != newColor.r || text_color.g != newColor.g ||
      text_color.b != newColor.b || text_color.a != newColor.a) {
    text_color = newColor;
  }
  return *this;
}

void Button::click() {
  if (onClickHandler) {
    onClickHandler(*this);
  }
}

void Button::draw(Renderer* renderer) {
  auto& textRenderer = renderer->getTextRenderer();
  
  // bg
  if (is_pressed) {
    renderer->setDrawColor({255, 255, 255, 255});
  } else {
    renderer->setDrawColor({0, 0, 200, 255});
  }
  renderer->fillRect(x, y , width, height);

  renderer->setDrawColor({50, 50, 50, 255});
  renderer->drawRect(x, y, width, height);

  auto f = textRenderer.loadFont(font_path, font_size);
  auto textDim = textRenderer.measureText(label, f);
  int textX = x + (width - textDim.first) / 2;
  int textY = y + (height - textDim.second) / 2;

  textRenderer.loadFont(font_path, font_size);
  textRenderer.drawText(label, f, text_color, textX, textY, width, height, false);

  View::draw(renderer);
}

bool Button::handleProximaEvent(const ProximaEvent& event) {
  int absX = x;
  int absY = y;

  int mouseX = event.x;
  int mouseY = event.y;

  bool consumed = false;
  switch (event.type) {
    case MOUSE_PRESS:
      if (mouseX >= absX && mouseX < (absX + width) && mouseY >= absY &&
          mouseY < (absY + height)) {
        is_pressed = true;  // Mark button as pressed
        consumed = true;    // Consume the event
      }
      break;
    case MOUSE_RELEASE:
      if (is_pressed) {
        is_pressed = false;

        // Check if mouse is still within bounds on release
        if (mouseX >= absX && mouseX < (absX + width) && mouseY >= absY &&
            mouseY < (absY + height)) {
          // Trigger the click handler
          if (onClickHandler) {
            click();
          }
          consumed = true;  // Consume the event
        }
      }
      break;
    case WINDOW_RESIZE:
      break;
    default:
      break;
  }

  return View::handleProximaEvent(event);
}

}  // Namespace UI
