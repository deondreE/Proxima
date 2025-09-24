#pragma once
#include "ProximaEvent.hpp"
#include <queue>
#include <functional>
#include <array>
#include <SDL3/SDL.h>

class EventDispatcher {
public:
    using Handler = std::function<bool(const ProximaEvent&)>;
    
    EventDispatcher(SDL_Window* win);
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

  SDL_Window* w; 

  ProximaEvent translate(const SDL_Event& sdl_event);
};
