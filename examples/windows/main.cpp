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
  helloText->setFont("../../examples/windows/config/fonts/Delius-Regular.ttf", 22)
    .setColor({255, 255, 255, 255});

  auto btn = std::make_unique<Button>();
  btn->setFont("../../examples/windows/config/fonts/Delius-Regular.ttf", 22)
    .size(100, 50)
    .text("Working Btn")
    .setTextColor({0, 0, 0, 255})
    .onClick([](Button& btn) {
          std::cout << "Button Clicked" << std::endl;
          btn.text("Logged In");
          btn.setTextColor({255, 0, 0, 255});
    });

  auto line = std::make_unique<Line>(10,10,10,10);
  line->setColor({0, 255, 0, 255});
  line->pos(200, 200);

  auto rect = std::make_unique<Rect>();
  rect->size(100, 200);
  rect->setColor({200, 200, 200, 255});

  rootView.add(std::move(helloText));
  rootView.add(std::move(btn));
  rootView.add(std::move(rect));
  rootView.add(std::move(line));

  window.run();

  return 0;
}