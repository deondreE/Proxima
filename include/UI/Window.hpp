#pragma once

#if defined(__linux__)
    #include <SDL3/SDL.h>
    #include <SDL3_ttf/SDL_ttf.h>
#elif defined(_WIN32)
#include <Windows.h>
#include <windowsx.h>
#endif

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <utility>

#include "Renderer.hpp"
// #include "Core/EventDispatcher.hpp"
#include "Core/ProximaEvent.hpp"
#include "View.hpp"

namespace UI {

struct WindowConfig {
    std::string title = "Proxima UI Window";
    int initialWidth = 800; 
    int initialHeight = 600;
    bool resizable = true;
    bool vsync = true;
    #if defined(_WIN32)
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = 0;
    #endif
};

struct Titlebar {
  std ::string text;
  color_t bgColor;
  Color textColor;
};

/**
 * @brief Window interface for defining Window response.
 * T - Window
 * F_Window - Deleter Lambda, Window
 * R - Renderer
 * F_Renderer - Deleter Lambda, Renderer
 */
template <
    typename WindowHandleType, 
    typename WindowDeleterType,
    typename RendererHandleType,
    typename RendererDelterType>
class IWindow {
 public:
  std::unique_ptr<WindowHandleType, WindowDeleterType> _window;
  std::unique_ptr<RendererHandleType, RendererDelterType> _renderer;
  std::unique_ptr<View> _rootView;
  WindowConfig _config;
  int _titleBarHeight;
  
  IWindow(WindowHandleType* window, WindowDeleterType window_deleter,
          RendererHandleType* renderer, RendererDelterType renderer_deleter,
                   View* root_view_ptr,
                 const WindowConfig& config, int titleBarHeight)
      : _window(window, std::move(window_deleter)),
        _renderer(renderer, std::move(renderer_deleter)),
        _rootView(root_view_ptr),
        _config(config),
        _titleBarHeight(titleBarHeight) {}
  virtual ~IWindow() = default;
  
  virtual void run() = 0;
  virtual void stop() = 0;

  virtual bool initializePlatformSubsystems() = 0;
  virtual void cleanupPlatformSubsystems() = 0;

  [[nodiscard]] WindowHandleType* getWindow() const { return _window.get(); }
  [[nodiscard]] RendererHandleType* getRenderer() const { return _renderer.get(); }
  [[nodiscard]] View& getRootView() const { 
      if (!_rootView) {
        throw std::runtime_error("Brokey");
      }
      return *_rootView.get();
  }

  #pragma region Callbacks
  // template <typename Callback>
  // void onQuit(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onQuit(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onKeyPress(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onKeyPress(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onKeyRelease(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onKeyRelease(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onTextInput(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onTextInput(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onMousePress(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onMousePress(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onMouseRelease(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onMouseRelease(std::forward<Callback>(cb));
  //   }
  // }

  // template <typename Callback>
  // void onMouseMotion(Callback&& cb) {
  //   if (_eventDispatcher) {
  //     _eventDispatcher->onMouseMotion(std::forward<Callback>(cb));
  //   }
  // }
  #pragma endregion

protected:
    bool _running = false;
};

}  // Namespace UI