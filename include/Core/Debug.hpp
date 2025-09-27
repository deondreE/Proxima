#pragma once
#include "View.hpp"

#include <functional>
#include <memory>

namespace UI {
class Window;
class View;
class WindowConfig;
}  // namespace UI

namespace Core {
class DebugManager;

class Debug {
 public:
  static Debug& getInstance();

  Debug(const Debug&) = delete;
  Debug& operator=(const Debug&) = delete;
  Debug(Debug&&) = delete;
  Debug& operator=(Debug&&) = delete;

  void toggleDebugWindow();

  void openDebugWindow(const UI::WindowConfig& config = {});

  void closeDebugWindow();
  [[nodiscard]] bool isDebugWindowOpen() const;

  void toggleUIOutline();

  void setUIOutlineEnabled(bool enable);

  [[nodiscard]] bool isUIOutlineEnabled() const;

  void setSelectedView(UI::View* view);

  [[nodiscard]] UI::View* getSelectedView() const;
  void updateAndDrawDebugWindow();
private:
  Debug();
  ~Debug();

  std::unique_ptr<UI::Window> _debugWindow;
  Core::DebugManager& _debugManager;
};
}  // namespace Core
