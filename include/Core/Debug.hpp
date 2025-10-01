#pragma once
#include "UI/View.hpp"
#include "UI/Window.hpp"

#include <memory>

using namespace UI;

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

  Core::DebugManager& _debugManager;
};
}  // namespace Core
