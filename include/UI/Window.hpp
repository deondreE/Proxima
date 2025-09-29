#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include "Core/EventDispatcher.hpp"
#include "Core/ProximaEvent.hpp"
#include "View.hpp"

namespace UI {

struct WindowConfig {
    std::string title = "Proxima UI Window";
    int initialWidth = 800; 
    int initialHeight = 600;
    bool resizable = true;
    bool vsync = true;
};

/**
 * @brief Represents an application window, encapsulating (SDL_Window, SDL_Renderer,
 *        and the root UI View. It also manages SDL event polling and dispatching.)For Linux Environment;
 */
class Window {
private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _sdlWindow;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _sdlRenderer;

    std::unique_ptr<EventDispatcher> _eventDispatcher;
    std::unique_ptr<View> _rootView;
    WindowConfig _config;

    bool _running = false;

    TTF_Font* _titleBarFont = nullptr;
    int _titleBarHeight = 30;
    SDL_Color _titleBarBgColor = {50,50,50,255};
    SDL_Color _titleBarTextColor = {255, 255, 255, 255};


    bool initializeSDLSubsystems();

    void cleanupSDLSubsystems();

    bool handleWindowResizeEvents(const ProximaEvent& ev);

    bool handleQuitEvent(const ProximaEvent& ev);
public: 
    explicit Window(const WindowConfig& config = {});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] SDL_Window* getSDLWindow() const { return _sdlWindow.get(); }
    [[nodiscard]] SDL_Renderer* getSDLRenderer() const { return _sdlRenderer.get(); }
    [[nodiscard]] View& getRootView() const {
        if (!_rootView) {
            throw std::runtime_error("Root View not initialized in the window!");
        }
        return *_rootView;
    }

    void run();
    void stop();

    template<typename Callback>
    void onQuit(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onQuit(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onKeyPress(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onKeyPress(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onKeyRelease(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onKeyRelease(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onTextInput(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onTextInput(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onMousePress(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onMousePress(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onMouseRelease(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onMouseRelease(std::forward<Callback>(cb));
        }
    }

    template<typename Callback>
    void onMouseMotion(Callback&& cb) {
        if (_eventDispatcher) {
            _eventDispatcher->onMouseMotion(std::forward<Callback>(cb));
        }
    }

    // template<typename Callback>
    // void onMouseWheel(Callback&& cb) {
    //     if (_eventDispatcher) {
    //         _eventDispatcher->onMouseWheel(std::forward<Callback>(cb));
    //     }
    // }
};

}  // Namespace UI