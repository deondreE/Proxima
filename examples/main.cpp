#include <X11/Xlib.h>
#include <iostream>
#include "UI/Button.hpp"
#include "UI/ConfigManager.hpp"
#include "UI/InspectorPanel.hpp"
#include "UI/StackLayout.hpp"
#include "UI/Text.hpp"
#include "UI/View.hpp"

using namespace UI;

ConfigManager configManager;

void on_btn_clicked() {
  std::cout << "Button callback free!\n";
}

int main() {
  if (!configManager.load("./examples/config/app_config.yaml")) {
    return 1;
  }

  const AppConfig& appConfig = configManager.getAppConfig();

  Display* dpy = XOpenDisplay(NULL);
  if (!dpy) {
    std::cerr << "Failed to open display\n";
    return 1;
  }

  int screen = DefaultScreen(dpy);
  Window win =
      XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 100, 100,
                          appConfig.initial_width, appConfig.initial_height, 1,
                          BlackPixel(dpy, screen), WhitePixel(dpy, screen));
  XStoreName(dpy, win, appConfig.window_title.c_str());
  XSelectInput(dpy, win,
               ExposureMask | KeyPressMask | ButtonPressMask |
                   StructureNotifyMask | ConfigureNotify);
  XMapWindow(dpy, win);

  Atom wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(dpy, win, &wm_delete_window, 1);

  GC gc = XCreateGC(dpy, win, 0, NULL);

  // Built UI
  Text hello("test");
  hello.size(200, 30);
  Button btn;
  btn.size(120, 40)
      .pos(100, 100)
      .text("Testing this button")
      .onClick(on_btn_clicked);

  StackLayout layout;
  layout.orientation(Vertical).spacing(20);
  layout.add(hello);
  layout.add(btn);

  View root;
  root.size(appConfig.initial_width, appConfig.initial_height).add(layout);

  InspecterPanel inspecterPanel;
  inspecterPanel.setInspectedRoot(&root);

  bool running = true;
  XEvent e;

  while (running) {
    XNextEvent(dpy, &e);

    inspecterPanel.handleEvent(e);

    switch (e.type) {
      case Expose: {
        XClearWindow(dpy, win);
        root.layout(10, 20);
        root.draw(dpy, win, gc);
        inspecterPanel.draw(dpy, win, gc);
        XFlush(dpy);
        break;
      }
      case ButtonPress: {
        if (inspecterPanel.Visible()) {
          int mx = e.xbutton.x;
          int my = e.xbutton.y;

          if (mx >= btn.x && mx <= btn.x + btn.width && my >= btn.y &&
              my <= btn.y + btn.height) {
            btn.click();
          }
        }
        break;
      }
      case KeyPress:
        break;
      case ConfigureNotify: {
        if (e.xconfigure.window == win) {
          root.size(appConfig.initial_width, appConfig.initial_height);

          XEvent expose_event;
          expose_event.type = Expose;
          expose_event.xexpose.window = win;
          expose_event.xexpose.x = 0;
          expose_event.xexpose.y = 0;
          expose_event.xexpose.width = e.xconfigure.width;
          expose_event.xexpose.height = e.xconfigure.height;
          expose_event.xexpose.count = 0;
          XSendEvent(dpy, win, False, ExposureMask, &expose_event);
        }
        break;
      }
      case ClientMessage: {
        if ((Atom)e.xclient.data.l[0] == wm_delete_window) {
          running = false;
        }
        break;
      }
      default:
        break;
    }
  }

  XFreeGC(dpy, gc);
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
  return 0;
}
