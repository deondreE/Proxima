#include <iostream>
#include "Proxima.hpp"
#include <filesystem>
using namespace UI;

Core::ConfigManager configManager;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
  if (!configManager.load("../../examples/windows/config/app_config.yaml")) {
    std::cerr << "Failed to load config\n";
    return 1;
  }
  const Core::AppConfig& appConfig = configManager.getAppConfig();

  WindowConfig cfg; 
  cfg.title = appConfig.window_title;
  cfg.initialWidth = appConfig.initial_width;
  cfg.initialHeight = appConfig.initial_height;
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


  auto backgroundImage = std::make_unique<Image>();
backgroundImage->pos(0, 0)
    .size(100, 100)
    .pos(10, 10) 
    .setImagePath("../../examples/windows/config/assets/test-app.png");

  auto line = std::make_unique<Line>(10,10,10,10);
  line->setColor({0, 255, 0, 255});
  line->pos(200, 200);

  auto slider = std::make_unique<Slider>(50, 400, 300, 0.0f, 100.0f, 75.0f); 
  slider->setStep(5.0f);
  slider->setColors({100, 100, 200, 255}, {255, 150, 0, 255});
  slider->setDimentions(30, 20, 30);
  slider->onValueChanged([](float newValue) {
    std::cout << "Volume changed to: " << newValue << std::endl;
  });
  slider->z_index(10);

  auto textInput = std::make_unique<TextInput>();
  textInput->setFont("../../examples/windows/config/fonts/Delius-Regular.ttf", 22);
  textInput->setColor({255, 0, 0, 255});
  textInput->pos(100, 100);

  rootView.add(std::move(helloText));
  rootView.add(std::move(btn));
  rootView.add(std::move(backgroundImage));
  rootView.add(std::move(line));
  rootView.add(std::move(slider));
  rootView.add(std::move(textInput));

  window.run();

  return 0;
}