#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <memory>

#include "UI/Button.hpp"
#include "UI/ConfigManager.hpp"
#include "UI/StackLayout.hpp"
#include "UI/Text.hpp"
#include "UI/View.hpp"
#include "UI/TextInput.hpp"

#include "Core/EventDispatcher.hpp" 

using namespace UI;

void on_btn_clicked() {
  std::cout << "Button clicked!\n";
}

ConfigManager configManager;
int main(int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  if (!TTF_Init()) {
    std::cerr << "SDL_ttf could not initialize! TTF_Error: " << std::endl;
    SDL_Quit();  // Ensure SDL is also quit if TTF fails
    return 1;
  }

  if (!configManager.load("./examples/config/app_config.yaml")) {
    std::cerr << "Failed to load config\n";
    SDL_Quit();
    TTF_Quit();
    return 1;
  }
  const AppConfig& appConfig = configManager.getAppConfig();

  SDL_Window* window =
      SDL_CreateWindow("SDL3 Test Window", appConfig.initial_width,
                       appConfig.initial_height, SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    TTF_Quit();
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 1;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  Text hello("Hello Proxima UI");
  hello.size(300, 30);
  hello.pos(0, 100);
  hello.setFont("./examples/config/fonts/Delius-Regular.ttf", 20);
  hello.setColor({0, 0, 0, 255});

  Text hello2("Another Text Item (higher z and wrapped)");
  hello2.size(350, 20);
  hello2.setFont("./examples/config/fonts/Delius-Regular.ttf", 28);
  hello2.setColor({0, 0, 0, 255});
  hello2.z_index(24);
  hello2.setWordWrap(true);

  Button btn;
  btn.size(120, 40)
      .pos(10, 60)
      .setFont("./examples/config/fonts/Delius-Regular.ttf", 36)
      .text("Press this button!")
      .onClick(on_btn_clicked);
  btn.setColor({255, 255, 255, 255});

  TextInput inputField;
  inputField.pos(10, 10); 
  inputField.size(380, 40); 
  inputField.setFont("./examples/config/fonts/Delius-Regular.ttf", 24);
  inputField.setColor({0, 0, 0, 255});
  inputField.setCursorColor({0, 0, 255, 255});
  inputField.setBackground({230, 230, 230, 255});
  inputField.text("Type here...");

  StackLayout layout;
  layout.orientation(Vertical).spacing(20);
  layout.pos(100, 100);
  layout.add(btn);
  layout.add(hello2);
  layout.add(hello);
  layout.add(inputField);

  View root;
  root.size(appConfig.initial_width, appConfig.initial_height);
  root.pos(0, 0);
  root.add(layout);

  // Event Dispatcher
  EventDispatcher eventDispatcher(window);
  bool running = true;

  eventDispatcher.onQuit([&](const ProximaEvent& ev) {
    running = false;
    return true; // Consume the event
  });

  eventDispatcher.onKeyPress([&](const ProximaEvent& ev) {
    if (ev.keyCode == SDLK_ESCAPE) {
      running = false;
      return true;
    }

    root.handleProximaEvent(ev);
    return false; // Don't consume here, let others potentially handle it
  });

  eventDispatcher.onTextInput([&](const ProximaEvent& ev) {
    // Forward text input to the root UI view
    root.handleProximaEvent(ev);
    return false; // Not consumed at this top level, allow UI components to handle
  });

   eventDispatcher.onMousePress([&](const ProximaEvent& ev) {
    root.handleProximaEvent(ev);
    return false;
  });

  eventDispatcher.onMouseRelease([&](const ProximaEvent& ev) {
    root.handleProximaEvent(ev);
    return false;
  });

  eventDispatcher.onMouseMotion([&](const ProximaEvent& ev) {
    root.handleProximaEvent(ev);
    return false;
  });

  eventDispatcher.onWindowResize([&](const ProximaEvent& ev) {
      // You'll likely need to pass this to your root view
      root.size(ev.width, ev.height); // Or similar
      root.handleProximaEvent(ev);
      return false;
  });

  while (running) {
    eventDispatcher.pollAndTranslate();
    eventDispatcher.dispatch();
    // Clear with blue background
    SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
    SDL_RenderClear(renderer);

    root.layout(0, 0);
    root.draw(renderer);

    SDL_RenderPresent(renderer);  // Update the screen
  }

  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}