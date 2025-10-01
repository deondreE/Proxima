#pragma once
#include <initializer_list>
#include "View.hpp"

namespace UI {
enum Orientation { Vertical, Horizontal };

class StackLayout : public View {
  Orientation orient{Vertical};
  int spacing_val{10};

 public:
  StackLayout();
  ~StackLayout() override = default;

  StackLayout& orientation(Orientation o);
  StackLayout& spacing(int s);
  StackLayout& children_list(std::initializer_list<View*> vs);

  void draw(const ViewContext& context) override;
  void layout(int ox, int oy) override;
  bool handleProximaEvent(const ProximaEvent& event) override;
};

}  // Namespace UI
