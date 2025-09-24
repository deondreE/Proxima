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
  int currentChildPosX = this->x;
  int currentChildPosY = this->y;

  int totalWidth;
  int totalHeight;

  for (auto& c : children) {
    if (!c)
      continue;
    c->layout(currentChildPosX, currentChildPosY);
    if (orient == Vertical) {
      currentChildPosY += c->height + spacing_val;
      totalWidth = std::max(totalWidth, c->width);
      totalHeight += c->height + spacing_val;
    } else {
      currentChildPosX += c->width + spacing_val;
      totalWidth += c->width + spacing_val;
      totalHeight = std::max(totalHeight, c->height);
    }

    if (!children.empty()) {
      if (orient == Vertical) {
        this->width = totalWidth;
        this->height = totalHeight - spacing_val;
      } else {
        this->width = totalWidth - spacing_val;
        this->height = totalHeight;
      }
    } else {
      this->width = 0;
      this->height = 0; 
    }
  }
}

void StackLayout::draw(SDL_Renderer* renderer) {
  for (auto& child : children) {
    child->draw(renderer);
  }
  View::draw(renderer);
}

void StackLayout::handleEvent(const SDL_Event& event) {
  for (auto& child : children) {
    if (child) {
      child->handleEvent(event);
    } 
  }
}

}  // Namespace UI
