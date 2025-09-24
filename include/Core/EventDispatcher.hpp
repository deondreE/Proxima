#pragma once
#include "ProximaEvent.hpp"
#include <vector>
#include <queue>
#include <functional>
#include <map>

namespace UI {
    class View;
}

class EventDispatcher {
public:
    using EventHandler = std::function<bool(ProximaEvent&)>;

private:
    std::queue<ProximaEvent> eventQueue;
    EventHandler globalHandler;
    std::map<KeySym, ProximaEventKeyCode> keySymToCode;
    std::map<KeySym, std::string> keySymToString;

public:
    EventDispatcher();
    ~EventDispatcher() = default;

    void setGlobalHandler(EventHandler handler);
    EventHandler getGlobalEventHandler(); 
    void pollAndTranslateEvents(Display* dpy);

    void dispatchEvents();

private:
    // Helper to translate X11 KeySyms
    void initializeKeySymMap();
    ProximaEventKeyCode getKeySymCode(KeySym keysym);
    std::string getKeySymString(KeySym keysym);
    
    // Helper to translate raw XEvent to ProximaEvent
    ProximaEvent translateEvent(const XEvent& xevent);
};