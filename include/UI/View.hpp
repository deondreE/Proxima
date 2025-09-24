#pragma once
#include <SDL3/SDL.h>
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

  virtual void draw(SDL_Renderer* renderer);

  virtual void layout(int offsetX, int offsetY);
  virtual void handleEvent(const SDL_Event& event);
};

}  // Namespace UI
