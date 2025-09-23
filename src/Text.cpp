#include "UI/Text.hpp"
#include <iostream>

namespace UI {

Text::Text(const std::string& str) : content(str) {}

Text& Text::text(const std::string& str) {
  content = str;
  return *this;
}

void Text::draw(Display* dpy, Window win, GC gc) {
  XDrawString(dpy, win, gc, x, y + 15, content.c_str(), content.length());
  View::draw(dpy, win, gc);
}

}  // Namespace UI
