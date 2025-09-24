#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>

#include "../Core/ProximaEvent.hpp"

namespace UI {

class View {
 private:
  View* parent = nullptr;

  void insertChildSorted(View* child);

 public:
  int x{0}, y{0}, width{100}, height{30}, zIndex{0};

  std::vector<View*> children;

  View() = default;
  virtual ~View() = default;

  View& size(int w, int h);
  View& pos(int nx, int ny);
  View& z_index(int z);

  template <typename T>
  T& add(T& child) {
    children.push_back(&child);
    return child;
  }

  View& add(std::initializer_list <View*> newChildren) {
    for (View* child : newChildren) {
      if (child) {
        children.push_back(child);
        child->parent = this;
      }
    }
    return *this;
  }

  virtual void draw(SDL_Renderer* renderer);
  virtual void layout(int offsetX, int offsetY);
  virtual void handleEvent(const SDL_Event& event);
  virtual bool handleProximaEvent(const ProximaEvent& event);

  View(const View&) = delete;
  View& operator=(const View&) = delete;
  View(View&&) = delete;
  View& operator=(View&&) = delete;
};

}  // Namespace UI
