#include "UI/View.hpp"
#include <algorithm>
#include <iostream>

namespace UI {

View& View::z_index(int z) {
  zIndex = z;
  return *this;
}

void View::insertChildSorted(std::unique_ptr<View> child) {
  auto it = std::lower_bound(
      children.begin(), children.end(), child,
      [](const std::unique_ptr<View>& a, const std::unique_ptr<View>& b) {
        if (a->zIndex != b->zIndex)  {
          return a->zIndex < b->zIndex;
        }
        return a.get() < b.get();
      });
  children.insert(it, std::move(child));
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
  for (const auto& child_ptr : children) {
    if (child_ptr) {
      child_ptr->draw(renderer);
    }
  }
}

void View::layout(int offsetX, int offsetY) {
  int absoluteX = x + offsetX;
  int absoluteY = y + offsetY;

  for (const auto& child_ptr : children) {
    if (child_ptr) {
      child_ptr->layout(absoluteX, absoluteY);
    }
  }
}

bool View::handleProximaEvent(const ProximaEvent& event) {
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    if (*it && (*it)->handleProximaEvent(event)) {
      return true;
    }
  }

  switch (event.type) {
    case KEY_PRESS:
    case KEY_RELEASE:
    case TEXT_INPUT:
      break;
    case MOUSE_PRESS:
         if (event.x >= x && event.x < (x + width) &&
                event.y >= y && event.y < (y + height)) {
                  return true;
                }
          break;
    case MOUSE_RELEASE:
      break;
    case MOUSE_MOTION:
          break;
    case WINDOW_RESIZE:
          break;
    case QUIT:
    case NONE:
        break;
  }
  return false; 
}

}  // Namespace UI
