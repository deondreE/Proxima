#include "UI/Button.hpp"
#include <iostream>

namespace UI {

Button& Button::text(const std::string& str) {
  label = str;
  return *this;
}

Button& Button::onClick(std::function<void()> cb) {
  onClickHandler = cb;
  return *this;
}

void Button::click() {
  if (onClickHandler) {
    onClickHandler();
  }
}

void Button::draw(Display* dpy, Window win, GC gc) {
  XDrawRectangle(dpy, win, gc, x, y, width, height);
  XDrawString(dpy, win, gc, x + 5, y + height / 2, label.c_str(),
              static_cast<int>(label.length()));
  View::draw(dpy, win, gc);
}

}  // Namespace UI
