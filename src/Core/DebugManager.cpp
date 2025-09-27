#include "Core/DebugManager.hpp"

namespace Core {
DebugManager& DebugManager::getInstance() {
  static DebugManager instance;
  return instance;
}

void DebugManager::setSelectedView(UI::View* view) {
  if (_selectedView) {
      _selectedView = view;
      if (_onSelectionChanged) {
          _onSelectionChanged(view);
      }
  }
}
}  // Namespace Core
