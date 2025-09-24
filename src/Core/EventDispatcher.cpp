#include "Core/EventDispatcher.hpp"
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/keysym.h>
#include "Core/ProximaEvent.hpp"

EventDispatcher::EventDispatcher(SDL_Window* win) : w(win) {  
    SDL_StartTextInput(w);
}
  
EventDispatcher::~EventDispatcher() {
    SDL_StopTextInput(w);
}

void EventDispatcher::onQuit(Handler h)         { _handlers[QUIT] = std::move(h); }
void EventDispatcher::onKeyPress(Handler h)     { _handlers[KEY_PRESS] = std::move(h); }
void EventDispatcher::onKeyRelease(Handler h)   { _handlers[KEY_RELEASE] = std::move(h); }
void EventDispatcher::onMousePress(Handler h)   { _handlers[MOUSE_PRESS] = std::move(h); }
void EventDispatcher::onMouseRelease(Handler h) { _handlers[MOUSE_RELEASE] = std::move(h); }
void EventDispatcher::onMouseMotion(Handler h)  { _handlers[MOUSE_MOTION] = std::move(h); }
void EventDispatcher::onWindowResize(Handler h) { _handlers[WINDOW_RESIZE] = std::move(h); }
void EventDispatcher::onTextInput(Handler h)    { _handlers[TEXT_INPUT] = std::move(h); }

void EventDispatcher::pollAndTranslate() {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        _q.push(translate(sdl_event));
    }
}

void EventDispatcher::dispatch() {
    while (!_q.empty()) {
        const auto ev = _q.front();
        _q.pop();
        auto& h = _handlers[ev.type];
        if (h && h(ev)) {
            continue;
        }
    }
}

ProximaEvent EventDispatcher::translate(const SDL_Event& sdl_event) {
    ProximaEvent e;
    switch (sdl_event.type) {
        case SDL_EVENT_QUIT:
            e.type = QUIT;
            break;
        case SDL_EVENT_KEY_DOWN:
            e.type = KEY_PRESS;
            e.keyCode = static_cast<ProximaEventKeyCode>(sdl_event.key.key);
            e.modifiers = static_cast<ProximaEventModifiers>(sdl_event.key.mod);
            break;
        case SDL_EVENT_KEY_UP:
            e.type = KEY_RELEASE;
            e.keyCode = static_cast<ProximaEventKeyCode>(sdl_event.key.key);
            e.modifiers = static_cast<ProximaEventModifiers>(sdl_event.key.mod);
            break;
        case SDL_EVENT_TEXT_INPUT:
            e.type = TEXT_INPUT;
            e.text = sdl_event.text.text;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            e.type = MOUSE_PRESS;
            e.button = static_cast<ProximaEventMouseButton>(sdl_event.button.button);
            e.x = sdl_event.button.x;
            e.y = sdl_event.button.y;
            e.modifiers = static_cast<ProximaEventModifiers>(sdl_event.button.which); // SDL_MouseButtonEvent has 'state' for modifiers
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            e.type = MOUSE_RELEASE;
            e.button = static_cast<ProximaEventMouseButton>(sdl_event.button.button);
            e.x = sdl_event.button.x;
            e.y = sdl_event.button.y;
            e.modifiers = static_cast<ProximaEventModifiers>(sdl_event.button.which);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            e.type = MOUSE_MOTION;
            e.x = sdl_event.motion.x;
            e.y = sdl_event.motion.y;
            e.modifiers = static_cast<ProximaEventModifiers>(sdl_event.motion.state);
            break;
        case SDL_EVENT_WINDOW_SHOWN:
            if (sdl_event.window.type == SDL_EVENT_WINDOW_RESIZED) {
                e.type = WINDOW_RESIZE;
                e.width = sdl_event.window.data1;
                e.height = sdl_event.window.data2;
            } else {
                e.type = NONE; // Ignore other window events for simplicity
            }
            break;
        default:
            e.type = NONE;
            break;
    }
    return e;
}