#include "UI/InspectorPanel.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include "UI/View.hpp"

namespace UI {

InspecterPanel::InspecterPanel() {
  this->pos(inspectorPanelX, 0);
  this->size(inspectorPanelWidth, 800);

  isVisible = false;
  inspectedRoot = nullptr;
  selectedNode = nullptr;
  uiTreeData = {};
  uiTreeData.typeName = "Root";
  uiTreeData.x = 0;
  uiTreeData.y = 0;
  uiTreeData.width = inspectorPanelWidth;
  uiTreeData.height = 800;
}

void InspecterPanel::toggle() {
  isVisible = !isVisible;
  if (isVisible) {
    refreshTreeData();
  } else {
    selectedNode = nullptr;
  }
}

void InspecterPanel::setInspectedRoot(View* root) {
  inspectedRoot = root;
  if (isVisible) {
    refreshTreeData();
  }
}

void InspecterPanel::refreshTreeData() {
  if (inspectedRoot) {
    uiTreeData = buildInspectNode(inspectedRoot);
    selectedNode = nullptr;
  } else {
    uiTreeData = {};
    uiTreeData.typeName = "No Root";
    uiTreeData.width = inspectorPanelWidth;
    uiTreeData.height = 800;
    selectedNode = nullptr;
  }
}

InspectNode InspecterPanel::buildInspectNode(View* view) {
  if (!view)
    return {};

  InspectNode node;
  node.typeName = typeid(*view).name();

  node.x = view->x;
  node.y = view->y;
  node.width = view->width;
  node.height = view->height;

  for (auto* child_ptr : view->children) {
    if (child_ptr) {
      node.children.push_back(buildInspectNode(child_ptr));
    }
  }

  return node;
}

void InspecterPanel::draw(Display* dpy, Window win, GC gc) {
  if (!isVisible || !inspectedRoot) {
    return;
  }

  drawInspectorUI(dpy, win, gc);
  drawOverlay(dpy, win, gc);
}

void InspecterPanel::drawInspectorUI(Display* dpy, Window win, GC gc) {
  XGCValues currentGCVals;
  XGetGCValues(dpy, gc, GCForeground | GCFillStyle, &currentGCVals);

  // Draw panel bg
  XGCValues gcv_panel_bg;
  gcv_panel_bg.foreground = 0xcccccc;  // Light gray for panel background
  gcv_panel_bg.fill_style = FillSolid;
  XChangeGC(dpy, gc, GCForeground | GCFillStyle, &gcv_panel_bg);
  XFillRectangle(dpy, win, gc, x, y, width, height);
  XDrawRectangle(dpy, win, gc, x, y, width, height);  // border
  int currentY = y + 20;

  XGCValues gcv_text;
  gcv_text.foreground = 0x333333;  // Dark gray for text
  XChangeGC(dpy, gc, GCForeground, &gcv_text);

  std::function<void(const InspectNode&, int)> drawTreeNode =
      [&](const InspectNode& node, int depth) {
        if (currentY >= (y + height - 30))
          return;

        std::string nodeInfo = node.typeName + " (" + std::to_string(node.x) +
                               "," + std::to_string(node.y) + ")";
        if (node.width != 100 ||
            node.height != 30) {  // Don't clutter if default size
          nodeInfo += " [" + std::to_string(node.width) + "x" +
                      std::to_string(node.height) + "]";
        }

        XGCValues savedGCValues;
        XGetGCValues(dpy, gc, GCForeground, &savedGCValues);

        if (&node == selectedNode) {
          XGCValues nodeGcv;
          nodeGcv.foreground = 0x0000ff;
          XChangeGC(dpy, gc, GCForeground, &nodeGcv);
        }

        XDrawString(dpy, win, gc, x + depth * 15 + 10, currentY,
                    nodeInfo.c_str(), nodeInfo.length());
        currentY += 20;

        for (const auto& childNode : node.children) {
          drawTreeNode(childNode, depth + 1);
        }
        XChangeGC(dpy, gc, GCForeground, &savedGCValues);
      };
  drawTreeNode(uiTreeData, 0);

  currentY += 10;
  XGCValues gcv_separator;
  gcv_separator.foreground = 0x888888;
  XChangeGC(dpy, gc, GCForeground, &gcv_separator);
  XDrawLine(dpy, win, gc, x, currentY, x + width, currentY);
  currentY += 10;

  XGCValues gcv_props_text;
  gcv_props_text.foreground = 0x333333;
  XChangeGC(dpy, gc, GCForeground, &gcv_props_text);

  if (selectedNode) {
    std::string props = "Selected: " + selectedNode->typeName;
    props += " Pos: (" + std::to_string(selectedNode->x) + "," +
             std::to_string(selectedNode->y) + ")";
    props += " Size: [" + std::to_string(selectedNode->width) + "x" +
             std::to_string(selectedNode->height) + "]";
    XDrawString(dpy, win, gc, x + 10, currentY, props.c_str(), props.length());
  } else {
    XDrawString(dpy, win, gc, x + 10, currentY, "Select a node", 14);
  }

  XChangeGC(dpy, gc, GCForeground | GCFillStyle, &currentGCVals);
}

void InspecterPanel::drawOverlay(Display* dpy, Window win, GC gc) {
  if (!isVisible || !selectedNode) {
    return;
  }

  XGCValues gcv_overlay;
  XGCValues currentGCVals;
  XGetGCValues(dpy, gc, GCForeground | GCLineWidth, &currentGCVals);

  gcv_overlay.foreground = 0xff0000;
  gcv_overlay.line_width = 2;
  XChangeGC(dpy, gc, GCForeground | GCLineWidth, &gcv_overlay);

  XDrawRectangle(dpy, win, gc, selectedNode->x, selectedNode->y,
                 selectedNode->width, selectedNode->height);

  XChangeGC(dpy, gc, GCForeground | GCLineWidth, &gcv_overlay);
}

void InspecterPanel::handleEvent(XEvent& event) {
  if (!isVisible) {
    if (event.type == KeyPress) {
      KeySym keysym = XLookupKeysym(&event.xkey, 0);
      if (keysym == XK_i) {
        toggle();
      }
    }
    return;
  }

  if (event.type == ButtonPress) {
    int mouseX = event.xbutton.x;
    int mouseY = event.xbutton.y;

    if (mouseX >= this->x && mouseX <= this->x + this->width &&
        mouseY >= this->y && mouseY <= this->y + this->height) {
      if (!selectedNode) {
        selectedNode = &uiTreeData;
      }
    }
  }
  if (event.type == KeyPress) {
    KeySym keysym = XLookupKeysym(&event.xkey, 0);
    if (keysym == XK_Escape) {  // Escape key to close inspector
      isVisible = false;
      selectedNode = nullptr;
    }
    if (keysym == XK_i) {
      toggle();
    }
  }
}

}  // Namespace UI
