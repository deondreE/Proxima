#pragma once
#include "ProximaEvent.hpp"
#include "UI/Window.hpp"
#include <queue>
#include <functional>
#include <array>


#if defined(_WIN32)
#include <cstdint>
#elif defined(__linux__)
#include <SDL3/SDL.h>
#endif

namespace UI {
    class View;
}

class EventDispatcher {
public:
    using Handler = std::function<bool(const ProximaEvent&)>;
    
    EventDispatcher(Window* win);
    ~EventDispatcher();

    void onQuit       (Handler h);
    void onKeyPress   (Handler h);
    void onKeyRelease (Handler h);
    void onMousePress (Handler h);
    void onMouseRelease(Handler h);
    void onMouseMotion(Handler h);
    void onWindowResize(Handler h);
    void onTextInput(Handler h);

    void  pollAndTranslate();
    void  dispatch();
private:
  std::queue<ProximaEvent> _q;
  std::array<Handler, TEXT_INPUT + 1> _handlers;

  Window* w; 

  ProximaEvent translate(const SDL_Event& sdl_event);
};
