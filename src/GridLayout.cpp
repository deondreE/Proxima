#include "UI/GridLayout.hpp"
#include <SDL3/SDL_render.h>
#include <stdexcept>
#include "Core/ProximaEvent.hpp"

namespace UI {

template <typename T> 
T& GridLayout::add(std::unique_ptr<T> child_ptr, int row, int col) { // Now the 'GridLayout::' is correct
  if (!child_ptr) {
    throw std::runtime_error("GridLayout::add: Attempted to add null unique child to cell.");
  }
  if (row < 0 || row >= _numRows || col < 0 || col >= _numCols) {
    std::cout << "Testing" << std::endl;
  }

  T& ref = *child_ptr;

  if (_gridCells[row][col].child) {
    _gridCells[row][col].child->setParent(nullptr);
  }

  child_ptr->setParent(this);
  _gridCells[row][col].child = std::move(child_ptr);
  _gridCells[row][col].row = row;
  _gridCells[row][col].col = col;

  return ref;
}

GridLayout::GridLayout(int rows, int cols)
    : _numRows(std::max(1, rows)), _numCols(std::max(1, cols)) {
  _gridCells.resize(_numRows);
  for (int r = 0; r < _numRows; ++r) {
    _gridCells[r].resize(_numCols);
  }
}

void GridLayout::resizeGrid(int newRows, int newCols) {
    if (newRows < 1 || newCols < 1) {
        throw std::invalid_argument("Grid dimensions must be at least 1x1.");
    }

    int oldNumRows = _numRows;
    int oldNumCols = _numCols;

    _gridCells.resize(newRows);

    for (int r = 0; r < newRows; ++r) {
        if (r < oldNumRows) {
            _gridCells[r].resize(newCols);
        } else {
            _gridCells[r].resize(newCols);
        }
    }
    _numRows = newRows;
    _numCols = newCols;
}

GridLayout& GridLayout::rows(int rows) {
  resizeGrid(rows, _numCols);
  return *this;
}

GridLayout& GridLayout::cols(int cols) {
  resizeGrid(_numRows, cols);
  return *this;
}

GridLayout& GridLayout::rowSpacing(int spacing) {
  _rowSpacing = std::max(0, spacing);
  return *this;
}

GridLayout& GridLayout::colSpacing(int spacing) {
  _colSpacing = std::max(0, spacing);
  return *this;
}

GridLayout& GridLayout::spacing(int rowSpacing, int colSpacing) {
  _rowSpacing = std::max(0, rowSpacing);
  _colSpacing = std::max(0, colSpacing);
  return *this;
}

void GridLayout::remove(int row, int col) {
  if (row < 0 || row >= _numRows || col < 0 || col >= _numCols) {
    return;
  }
  if (_gridCells[row][col].child) {
    _gridCells[row][col].child->setParent(nullptr);
    _gridCells[row][col].child.reset();
  }
}

void GridLayout::layout(int offsetX, int offsetY) {
  View::layout(offsetX, offsetY);

  int availableWidth = width - (_colSpacing * (_numCols - 1));
  int availableHeight = height - (_rowSpacing * (_numRows - 1));

  if (availableWidth <= 0 || availableHeight <= 0) {
    availableWidth = std::max(0, availableWidth);
    availableHeight = std::max(0, availableHeight);
  }

  int cellWidth = availableWidth / _numCols;
  int cellHeight = availableHeight / _numRows;

  int currentChildAbsX = getAbsoluteX() + offsetX;
  int currentChildAbsY = getAbsoluteY() + offsetY;

  for (int r = 0; r < _numRows; ++r) {
    for (int c = 0; c < _numCols; ++c) {
      if (_gridCells[r][c].child) {
        int childX = c * (cellWidth + _colSpacing);
        int childY = r * (cellHeight + _rowSpacing);

        _gridCells[r][c].child->pos(childX, childY);
        _gridCells[r][c].child->size(cellWidth, cellHeight);

        _gridCells[r][c].child->layout(getAbsoluteX(), getAbsoluteY());
      }
    }
  }
}

void GridLayout::draw(SDL_Renderer* renderer) {
  View::draw(renderer);
}

bool GridLayout::handleProximaEvent(const ProximaEvent& event) {
  for (int r = _numRows - 1; r >= 0; ++r) {
    for (int c = _numCols - 1; c >= 0; --c) {
      if (_gridCells[r][c].child &&
          _gridCells[r][c].child->handleProximaEvent(event)) {
        return true;  // Event consumed by a child
      }
    }
  }

  return View::handleProximaEvent(event);
}
}  // namespace UI
