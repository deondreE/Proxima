#pragma once
#include <string>
#include "View.hpp"

namespace UI {

class Text : public View {
  std::string content;

 public:
  explicit Text(const std::string& str);

  Text& text(const std::string& str);

  void draw(Display* dpy, Window win, GC gc) override;
};

}  // Namespace UI
