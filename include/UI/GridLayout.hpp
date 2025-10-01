#pragma once

#include "Core/ProximaEvent.hpp"
#include "UI/View.hpp"

#include <format>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace UI {

struct GridCell {
  std::unique_ptr<View> child;
  int row = -1;
  int col = -1;

  GridCell(std::unique_ptr<View> v = nullptr, int r = -1, int c = -1)
      : child(std::move(v)), row(r), col(c) {}

  GridCell(GridCell&& other) noexcept
      : child(std::move(other.child)), row(other.row), col(other.col) {
    other.row = -1;
    other.col = -1;
  }

  GridCell& operator=(GridCell&& other) noexcept {
    if (this != &other) {
      child = std::move(other.child);
      row = other.row;
      col = other.col;

      other.row = -1;
      other.col = -1;
    }
    return *this;
  }

  GridCell(const GridCell&) = delete;
  GridCell& operator=(const GridCell&) = delete;
};

class GridLayout : public View {
 private:
  int _numRows = 1;
  int _numCols = 1;
  int _rowSpacing = 0;
  int _colSpacing = 0;

  std::vector<std::vector<GridCell>> _gridCells;

  void resizeGrid(int newRows, int newCols);

 public:
  explicit GridLayout(int rows = 1, int cols = 1);
  ~GridLayout() override = default;

  GridLayout& rows(int rows);
  GridLayout& cols(int cols);
  GridLayout& rowSpacing(int spacing);
  GridLayout& colSpacing(int spacing);
  GridLayout& spacing(int rowSpace, int colSpace);

  template <typename T>
  T& add(std::unique_ptr<T> child_ptr, int row, int col);

  void remove(int row, int col);

  void draw(const ViewContext& context) override;
  void layout(int offsetX, int offsetY) override;
  bool handleProximaEvent(const ProximaEvent& event) override;
};

}  // Namespace UI
