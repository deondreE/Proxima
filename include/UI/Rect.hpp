#pragma once

#include "View.hpp"
#include "Color.hpp"
#include "Platform.hpp"

namespace UI {

class PEXPORT Rect : public View {
 private:
  Color color;
 public:
  Rect(int x = 0, int y = 0, int w = 100, int h = 100,
        const Color& c = {200, 200, 200, 255});        

  Rect& size(int w, int h) {
    View::size(w, h);
    return *this;
  }

  Rect& pos(int nx, int ny) {
    View::pos(nx, ny);
    return *this;
  }

  Rect& setColor(const Color& clr);

  void draw(Renderer* renderer) override;
};

}  // Namespace UI