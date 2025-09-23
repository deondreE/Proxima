#include <X11/Xlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(void) {
  Display* display;
  int screen;
  Window window;
  GC gc;
  XEvent event;
  bool running = true;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "ERROR: Cannot open X display\n");
    return 1;
  }

  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 100, 100,
                               WINDOW_WIDTH, WINDOW_HEIGHT, 1,
                               BlackPixel(display, screen),
                               WhitePixel(display, screen));

  XSelectInput(display, window,
               ExposureMask | KeyPressMask | StructureNotifyMask);
  XMapWindow(display, window);
  gc = XCreateGC(display, window, 0, NULL);

  while (running) {
    XNextEvent(display, &event);

    switch (event.type) {
      case Expose:
        XSetForeground(display, gc, WhitePixel(display, screen));
        XFillRectangle(display, window, gc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // Draw a red rect
        {
          unsigned long red = 0xff0000;
          XSetForeground(display, gc, red);
          XFillRectangle(display, window, gc, 50, 50, 200, 100);
        }

        XSetForeground(display, gc, BlackPixel(display, screen));
        XDrawRectangle(display, window, gc, 50, 50, 200, 100);
        XDrawString(display, window, gc, 60, 120, "Hello Proxima!", 14);
        break;

      case ConfigureNotify:
        printf(" Window resized to %dx%d\n", event.xconfigure.width,
               event.xconfigure.height);
        break;

      case KeyPress:
        printf("Key Pressed. Exiting\n");
        running = false;
        break;

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
