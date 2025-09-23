#include <X11/Xlib.h>
#include <iostream>
#include "UI/Button.hpp"
#include "UI/StackLayout.hpp"
#include "UI/Text.hpp"
#include "UI/View.hpp"

using namespace UI;

void on_btn_clicked() {
  std::cout << "Button callback free!";
}

int main() {
  Display* dpy = XOpenDisplay(NULL);
  if (!dpy) {
    std::cerr << "Failed to open display\n";
    return 1;
  }

  int screen = DefaultScreen(dpy);
  Window win =
      XCreateSimpleWindow(dpy, RootWindow(dpy, screen), 100, 100, 400, 300, 1,
                          BlackPixel(dpy, screen), WhitePixel(dpy, screen));
  XSelectInput(
      dpy, win,
      ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);
  XMapWindow(dpy, win);

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
  root.size(400, 300).add(layout);

  bool running = true;
  while (running) {
    XEvent e;
    XNextEvent(dpy, &e);

    switch (e.type) {
      case Expose: {
        XClearWindow(dpy, win);
        root.layout(10, 20);
        root.draw(dpy, win, gc);
        break;
      }
      case ButtonPress: {
        int mx = e.xbutton.x;
        int my = e.xbutton.y;

        if (mx >= btn.x && mx <= btn.x + btn.width && my >= btn.y &&
            my <= btn.y + btn.height) {
          btn.click();
        }
        break;
      }
      case KeyPress:
        running = false;
        break;
      default:
        break;
    }
  }

  XFreeGC(dpy, gc);
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
  return 0;
}
