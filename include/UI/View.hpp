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

  void insertChildSorted(std::unique_ptr<View> child);

 public:
  int x{0}, y{0}, width{100}, height{30}, zIndex{0};

  std::vector<std::unique_ptr<View>> children;

  View() = default;
  virtual ~View() = default;

  View& size(int w, int h);
  View& pos(int nx, int ny);
  View& z_index(int z);

  template <typename T>
  T& add(std::unique_ptr<T> child_ptr) {
    if (child_ptr) {
      child_ptr->parent = nullptr;
      T& ref = *child_ptr;
      insertChildSorted(std::move(child_ptr)); 
      return ref;
    }

    throw std::runtime_error("Attemted to add a null unique child to view.");
  }

  View& add(std::initializer_list<std::unique_ptr<View>> newChildren) = delete;

  virtual void draw(SDL_Renderer* renderer);
  virtual void layout(int offsetX, int offsetY);

  virtual bool handleProximaEvent(const ProximaEvent& event);

  View(const View&) = delete;
  View& operator=(const View&) = delete;
  View(View&&) = delete;
  View& operator=(View&&) = delete;
};

}  // Namespace UI
