#include "UI/View.hpp"
#include <algorithm>
#include <iostream>

namespace UI {

void View::draw() {
  std::cout << "Drawing Generic View (" << width << "x" << height << ")\n";
  for (auto& c : children) {
    if (c)
      c->draw();
  }
}

View& View::size(int w, int h) {
  width = w;
  height = h;
  return *this;
}

View& View::pos(int nx, int ny) {
  x = nx;
  y = ny;
  return *this;
}

void View::draw(SDL_Renderer* renderer) {
  for (auto& c : children) {
    if (c)
      c->draw(renderer);
  }
}

void View::layout(int offsetX, int offsetY) {
  x += offsetX;
  y += offsetY;
  for (auto& c : children) {
    if (c)
      c->layout(x, y);
  }
}

void View::handleEvent(const SDL_Event& event) {
  for (auto& child : children) {
    child->handleEvent(event);
  }
}

}  // Namespace UI
