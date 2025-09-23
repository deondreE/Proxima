#pragma once
#include <X11/Xlib.h>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace UI {

class View {
 public:
  int x{0}, y{0}, width{100}, height{30};
  std::vector<View*> children;

  virtual ~View() = default;

  virtual void draw();

  View& size(int w, int h);
  View& pos(int nx, int ny);

  template <typename T>
  T& add(T& child) {
    children.push_back(&child);
    return child;
  }

  virtual void draw(Display* dpy, Window win, GC gc);

  virtual void layout(int offsetX, int offsetY);
  virtual void handleEvent(XEvent& event);
};

}  // Namespace UI
