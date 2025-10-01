#include <iostream>
#include "Proxima.hpp"
using namespace UI;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
  WindowConfig cfg; 
  cfg.title = "Proxima Test Center";
  cfg.initialWidth = 800;
  cfg.initialHeight = 600;
  cfg.style = WS_OVERLAPPEDWINDOW;
  cfg.exStyle = 0;

  

  W_Window window(cfg);
  if (!window.initializePlatformSubsystems()) {
    MessageBoxA(nullptr, "Failed to initialize window subsystem", "Error:", MB_OK | MB_ICONERROR);
    return -1;
  }

  HWND hWnd = window.getWindow();
  if (hWnd) {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
  }

  View& rootView = window.getRootView();
  auto helloText = std::make_unique<Text>("Welcome to Proxima UI!");
  helloText->setFont("../../examples/windows/config/fonts/Delius-Regular.ttf", 22);

  rootView.add(std::move(helloText));

  window.run();

  return 0;
}