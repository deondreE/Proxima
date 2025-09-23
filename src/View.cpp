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

void View::draw(Display* dpy, Window win, GC gc) {
  for (auto& c : children) {
    if (c)
      c->draw(dpy, win, gc);
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

void View::handleEvent(XEvent& event) {
  for (auto& child : children) {
    child->handleEvent(event);
  }
}

}  // Namespace UI
