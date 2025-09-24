#include "UI/StackLayout.hpp"
#include <algorithm>
#include <iostream>

namespace UI {

StackLayout::StackLayout() = default;

StackLayout& StackLayout::orientation(Orientation o) {
  orient = o;
  return *this;
}

StackLayout& StackLayout::spacing(int s) {
  spacing_val = s;
  return *this;
}

void StackLayout::layout(int offsetX, int offsetY) {
  this->x = offsetX;
  this->y = offsetY;
  int currentX = x + offsetX;
  int currentY = y + offsetY;

  int maxChildWidth = 0;
  int maxChildHeight = 0;
  int totalContentSize = 0;

  for (const auto& child_ptr : children) {
    if (!child_ptr) continue;

    View* child = child_ptr.get();

    if (orient == Vertical) {
     child->pos(currentX, currentY + totalContentSize);
     totalContentSize += child->height + spacing_val;
     maxChildWidth = std::max(maxChildWidth, child->width);
    } else {
      child->pos(currentX + totalContentSize, currentY);
      totalContentSize += child->width + spacing_val;
      maxChildHeight = std::max(maxChildHeight, child->height);
    }

    child->layout(child->x, child->y);
  }
}

void StackLayout::draw(SDL_Renderer* renderer) {
  for (auto& child : children) {
    child->draw(renderer);
  }
  View::draw(renderer);
}

bool StackLayout::handleProximaEvent(const ProximaEvent& event) {
  return View::handleProximaEvent(event);
}

}  // Namespace UI
