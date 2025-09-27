#pragma once

#include <functional>
#include <string>
#include <vector>

namespace UI {
class View;
}  // Namespace UI

namespace Core {
class DebugManager {
 public:
  static DebugManager& getInstance();

  DebugManager(const DebugManager&) = delete;
  DebugManager& operator=(const DebugManager&) = delete;
  DebugManager(DebugManager&&) = delete;
  DebugManager& operator=(DebugManager&&) = delete;

  [[nodiscard]] bool isUIOutlineEnabled() const { return _uiOutlineEnabled; }
  void setUIOutlineEnabled(bool enable) { _uiOutlineEnabled = enable; }

  [[nodiscard]] UI::View* getSelectedView() const {
      return _selectedView;
  }

  void setSelectedView(UI::View* view);

  using SelectionChangedCallback = std::function<void(UI::View*)>;

  void setOnSelectionChangedCallback(SelectionChangedCallback cb) { _onSelectionChanged = std::move(cb); }
private:
    DebugManager() = default;

    bool _uiOutlineEnabled = false;
    UI::View* _selectedView = nullptr;

    SelectionChangedCallback _onSelectionChanged;
};
}  // Namespace Core
