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
  int layoutAbsX = offsetX + x;
  int layoutAbsY = offsetY + y;

  int currentChildPosX = layoutAbsX;
  int currentChildPosY = layoutAbsY;

  for (auto& c : children) {
    if (!c)
      continue;
    c->x = currentChildPosX;
    c->y = currentChildPosY;
    c->layout(currentChildPosX, currentChildPosY);
    offsetX +=
        (orient == Vertical ? c->height + spacing_val : c->width + spacing_val);
  }
}

void StackLayout::draw(Display* dpy, Window win, GC gc) {
  for (auto& child : children) {
    child->draw(dpy, win, gc);
  }
}

void StackLayout::handleEvent(XEvent& event) {
  for (auto& child : children) {
    child->handleEvent(event);
  }
}

}  // Namespace UI
