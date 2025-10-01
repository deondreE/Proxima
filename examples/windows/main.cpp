#include <iostream>
#include "Proxima.hpp"
using namespace UI;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
  WindowConfig cfg; 
  cfg.title = "Proxima Test Center";
  cfg.initialWidth = 800;
  cfg.initialHeight = 600;

  W_Window window(cfg);
  if (window.initializePlatformSubsystems()) {
    window.run();
  }

  return 0;
}