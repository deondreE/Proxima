#pragma once
#include <functional>
#include <string>
#include "View.hpp"

namespace UI {

class Button : public View {
  std::string label;
  std::function<void()> onClickHandler;

 public:
  Button() = default;

  Button& size(int w, int h) {
    View::size(w, h);
    return *this;
  }
  Button& pos(int nx, int ny) {
    View::pos(nx, ny);
    return *this;
  }
  Button& text(const std::string& str);
  Button& onClick(std::function<void()> cb);

  void click();
  void draw(Display* dpy, Window win, GC gc) override;
};
}  // Namespace UI
