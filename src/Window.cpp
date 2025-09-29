#include "UI/Window.hpp"
#include <format>
#include <iostream>

namespace UI {
auto sdl_window_deleter = [](SDL_Window* w) {
  if (w) {
    SDL_DestroyWindow(w);
  }
};

auto sdl_renderer_delter = [](SDL_Renderer* r) {
  if (r) {
    SDL_DestroyRenderer(r);
  }
};

Window::Window(const WindowConfig& config)
    : _sdlWindow(nullptr, sdl_window_deleter),
      _sdlRenderer(nullptr, sdl_renderer_delter),
      _config(config) {
  if (!initializeSDLSubsystems()) {
    throw std::runtime_error("Failed to init SDL subsystems for UI::Window.");
  }

  _sdlWindow.reset(SDL_CreateWindow(
      _config.title.c_str(), _config.initialWidth, _config.initialHeight,
      _config.resizable ? SDL_WINDOW_RESIZABLE : 0));
  if (!_sdlWindow) {
    cleanupSDLSubsystems();
    throw std::runtime_error(
        std::format("Failed to create SDL window: {}", SDL_GetError()));
  }

  _sdlRenderer.reset(SDL_CreateRenderer(_sdlWindow.get(), NULL));
  if (!_sdlRenderer) {
    _sdlWindow.reset();
    cleanupSDLSubsystems();
    throw std::runtime_error(
        std::format("Failed to create SDL renderer: {}", SDL_GetError()));
  }
  SDL_SetRenderDrawBlendMode(_sdlRenderer.get(), SDL_BLENDMODE_BLEND);

  _titleBarFont =
      TTF_OpenFont("./examples/config/fonts/Delius-Regular.ttf", 18);
  if (_titleBarFont) {
    SDL_Log("Failed to load font for window title bar.");
  }

  _rootView = std::make_unique<View>();
  _rootView->size(_config.initialWidth, _config.initialHeight);
  _eventDispatcher = std::make_unique<EventDispatcher>(_sdlWindow.get());

  _eventDispatcher->onWindowResize([this](const ProximaEvent& ev) {
    return this->handleWindowResizeEvents(ev);
  });
  _eventDispatcher->onQuit(
      [this](const ProximaEvent& ev) { return this->handleQuitEvent(ev); });
}

Window::~Window() {
  if (_titleBarFont) {
    TTF_CloseFont(_titleBarFont);
  }
  cleanupSDLSubsystems();
}

bool Window::initializeSDLSubsystems() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << std::format("SDL could not initialize! SDL_Error: {}\n",
                             SDL_GetError());
    return false;
  }

  if (!TTF_Init()) {
    std::cerr << std::format("SDL_ttf could not initialize! TTF_Error: {}\n",
                             "Test");
    SDL_Quit();  // Clean up SDL if TTF fails
    return false;
  }

  return true;
}

void Window::cleanupSDLSubsystems() {
  TTF_Quit();
  SDL_Quit();
}

bool Window::handleWindowResizeEvents(const ProximaEvent& ev) {
  if (ev.type == ProximaEventType::WINDOW_RESIZE) {
    _rootView->size(ev.width, ev.height);
    _rootView->handleProximaEvent(ev);
  }
  return false;
}

bool Window::handleQuitEvent(const ProximaEvent& ev) {
  if (ev.type == ProximaEventType::QUIT) {
    _running = false;
    return true;
  }
  return false;
}

void Window::stop() {
  _running = false;
}

void Window::run() {
  _running = true;

  // MAKE SURE the window  consumes every directed event, no need
  // to wait for the Dev to define these consumtion overloads.
  _eventDispatcher->onKeyPress([&](const ProximaEvent& ev) {
    if (ev.keyCode == SDLK_ESCAPE) {
      _running = false;
      return true;
    }

    _rootView->handleProximaEvent(ev);
    return false;  // Don't consume here, let others potentially handle it
  });

  _eventDispatcher->onTextInput([&](const ProximaEvent& ev) {
    // Forward text input to the root UI view
    _rootView->handleProximaEvent(ev);
    return false;  // Not consumed at this top level, allow UI components to handle
  });

  _eventDispatcher->onMousePress([&](const ProximaEvent& ev) {
    _rootView->handleProximaEvent(ev);
    return false;
  });

  _eventDispatcher->onMouseRelease([&](const ProximaEvent& ev) {
    _rootView->handleProximaEvent(ev);
    return false;
  });

  _eventDispatcher->onMouseMotion([&](const ProximaEvent& ev) {
    _rootView->handleProximaEvent(ev);
    return false;
  });

  while (_running) {
    Uint32 currentTime = SDL_GetTicks();
    _eventDispatcher->pollAndTranslate();
    _eventDispatcher->dispatch();

    SDL_SetRenderDrawColor(_sdlRenderer.get(), 100, 149, 237, 255);
    SDL_RenderClear(_sdlRenderer.get());

    int windowWidth, windowHeight;
    SDL_GetWindowSize(_sdlWindow.get(), &windowWidth, &windowHeight);

    SDL_FRect titleBarRect = {0, 0, (float)windowWidth, (float)_titleBarHeight};
    SDL_SetRenderDrawColor(_sdlRenderer.get(), _titleBarBgColor.r,
                           _titleBarBgColor.g, _titleBarBgColor.b,
                           _titleBarBgColor.a);
    SDL_RenderFillRect(_sdlRenderer.get(), &titleBarRect);

    if (_titleBarFont && !_config.title.empty()) {
      SDL_Surface* textSurface = TTF_RenderText_Blended(_titleBarFont, _config.title.c_str(), _config.title.length(), _titleBarTextColor);
      if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_sdlRenderer.get(), textSurface);
        if (textTexture) {
          SDL_FRect textRect;
          textRect.x = 5;
          textRect.y = (_titleBarHeight - textSurface->h) / 2.0f;
          textRect.w = textSurface->w;
          textRect.h = textSurface->h;

          SDL_RenderTexture(_sdlRenderer.get(), textTexture, nullptr, &textRect);
          SDL_DestroyTexture(textTexture);
        }
        SDL_DestroySurface(textSurface);
      }
    }

    _rootView->layout(0, _titleBarHeight);
    _rootView->draw(_sdlRenderer.get());

    SDL_RenderPresent(_sdlRenderer.get());
  }
}

}  // namespace  UI