
#include "Proxima.hpp"
using namespace UI;

void on_btn_clicked() {
  std::cout << "Button Clicked!\n";
}

ConfigManager configManager;

int main(int argc, char* argv[]) {

  if (!configManager.load("./examples/config/app_config.yaml")) {
    std::cerr << "Failed to load config\n";
    SDL_Quit();
    TTF_Quit();
    return 1;
  }
  const AppConfig& appConfig = configManager.getAppConfig();

  try {
    WindowConfig windowConfig;
    windowConfig.title = appConfig.window_title;
    windowConfig.initialHeight = appConfig.initial_height;
    windowConfig.initialWidth = appConfig.initial_width;
    windowConfig.resizable = true;
    windowConfig.vsync = true;

    Window mainWindow(windowConfig);

    View& rootView = mainWindow.getRootView();
    auto helloText = std::make_unique<Text>("Welcome to Proxima UI!");
    helloText->size(400, 30)
        .pos(20, 20)
        .setFont("./examples/config/fonts/Delius-Regular.ttf", 28)
        .setColor({50, 50, 50, 255});
    auto backgroundImage = std::make_unique<Image>();
    backgroundImage->pos(0, 0).size(appConfig.initial_width,
                                    appConfig.initial_height);
    backgroundImage->z_index(-100);
    backgroundImage->setImagePath("./examples/assets/test-app.png");

    auto btn = std::make_unique<Button>();
    btn->size(120, 40)
        .pos(10, 60)
        .setFont("./examples/config/fonts/Delius-Regular.ttf", 36)
        .text("Press this button!")
        .onClick(on_btn_clicked);
    btn->setColor({255, 255, 255, 255});

    auto volumeSlider =
        std::make_unique<Slider>(50, 400, 300, 0.0f, 100.0f, 75.0f);
    volumeSlider->setStep(5.0f);
    volumeSlider->setColors({100, 100, 200, 255}, {255, 150, 0, 255});
    volumeSlider->setDimentions(30, 20, 30);
    volumeSlider->onValueChanged([](float newValue) {
      std::cout << "Volume changed to: " << newValue << std::endl;
    });
    volumeSlider->z_index(10);

    rootView.add(std::move(helloText));
    rootView.add(std::move(backgroundImage));
    rootView.add(std::move(btn));
    rootView.add(std::move(volumeSlider));

    mainWindow.run();
  } catch (const std::runtime_error& e) {
    std::cerr << std::format("Application encountered an error: {}\n",
                             e.what());
    return 1;
  }

  std::cout << "Applicatio exited succesfully.";

  return 0;
}