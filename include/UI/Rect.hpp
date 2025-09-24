#pragma once
#include <SDL3/SDL.h>
#include "View.hpp"

namespace UI {

class Rect : public View {
 private:
  SDL_Color color;
 public:
  Rect(int x = 0, int y = 0, int w = 100, int h = 100,
         SDL_Color c = {200, 200, 200, 255});        

  Rect& size(int w, int h) {
    View::size(w, h);
    return *this;
  }

  Rect& pos(int nx, int ny) {
    View::pos(nx, ny);
    return *this;
  }

  Rect& setColor(SDL_Color clr);

  void draw(SDL_Renderer* renderer) override;
};

}  // Namespace UI