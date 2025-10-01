#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include <stdexcept>

#include "../Core/ProximaEvent.hpp"
#include "Renderer.hpp"
#include "Platform.hpp"
#include "ViewContext.hpp"

namespace UI {

class PEXPORT View {
 private:
  View* parent = nullptr;

  void insertChildSorted(std::unique_ptr<View> child);
protected:
  ViewContext _context; 

 public:
  int x{0}, y{0};
  int width{100}, height{30}, zIndex{0};

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
      child_ptr->setParent(this);
      if (_context.renderer) {
        child_ptr->setContext(_context);
      }
      insertChildSorted(std::move(child_ptr)); 
      return ref;
    }
    throw std::runtime_error("Attemted to add a null unique child to view.");
  }

  virtual void setContext(const ViewContext& context);
  const ViewContext& getContext() const { return _context; }

  void setParent(View* p);
  View* getParent() const { return parent; }

  View& add(std::initializer_list<std::unique_ptr<View>> newChildren) = delete;

  virtual void draw(const ViewContext& context);
  virtual void layout(int offsetX, int offsetY);
  virtual void update(float deltaTime, const ViewContext& context);

  virtual bool handleProximaEvent(const ProximaEvent& event);

  int getAbsoluteX() const;
  int getAbsoluteY() const;

  View(const View&) = delete;
  View& operator=(const View&) = delete;
  View(View&&) = delete;
  View& operator=(View&&) = delete;
};

}  // Namespace UI
