#pragma once

#include <memory>
#include <string>
#include <vector>
#include "View.hpp"

namespace UI {

struct InspectNode {
  std::string typeName;
  int x, y, width, height;
  std::vector<InspectNode> children;
  bool isSelected = false;
};

class InspecterPanel : public View {
  View* inspectedRoot = nullptr;

  InspectNode uiTreeData;
  InspectNode* selectedNode = nullptr;
  bool isVisible = false;

  int inspectorPanelWidth = 250;
  int inspectorPanelX = 800 - inspectorPanelWidth;

 public:
  InspecterPanel();
  ~InspecterPanel() override = default;

  void toggle();
  void setInspectedRoot(View* root);
  void refreshTreeData();
  bool Visible() { return isVisible; };

  void draw(Display* dpy, Window win, GC gc) override;

  void handleEvent(XEvent& event) override;

 private:
  InspectNode buildInspectNode(View* view);

  void drawInspectorUI(Display* dpy, Window win, GC gc);

  void drawOverlay(Display* dpy, Window win, GC gc);

  // bool hitTestInspector(int mouseX, int mouseY);
};

}  // Namespace UI
