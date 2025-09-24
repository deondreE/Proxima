#include "UI/View.hpp"
#include <algorithm>
#include <iostream>

namespace UI {

View& View::z_index(int z) {
  zIndex = z;
  return *this;
}

void View::insertChildSorted(View* child) {
  auto it = std::lower_bound(children.begin(), children.end(), child, 
    [](const View* a, const View* b) {
      return a->zIndex < b->zIndex;
    });
  children.insert(it, child);
  if (child) {
    child->parent = this;
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
  int absoluteX = x + offsetX;
  int absoluteY = y + offsetY;
  
  for (auto& c : children) {
    if (c)
      c->layout(absoluteX, absoluteY);
  }
}

void View::handleEvent(const SDL_Event& event) {
  for (auto& child : children) {
    child->handleEvent(event);
  }
}

}  // Namespace UI
