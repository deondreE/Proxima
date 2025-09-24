#include "Core/EventDispatcher.hpp" 
#include "UI/View.hpp" 
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <string>
#include <vector>

// key map init
#define ADD_KEYMAP(keysym, code, str) \
    keySymToCode[keysym] = code; keySymToString[keysym] = str;

EventDispatcher::EventDispatcher() {
    initializeKeySymMap();
}

void EventDispatcher::initializeKeySymMap() {
    ADD_KEYMAP(XK_BackSpace,  8,  "Backspace"); ADD_KEYMAP(XK_Tab,        9,  "Tab");
    ADD_KEYMAP(XK_Return,     13, "Enter");     ADD_KEYMAP(XK_Escape,     27, "Escape");
    ADD_KEYMAP(XK_space,      32, "Space");     ADD_KEYMAP(XK_Prior,      33, "PageUp");
    ADD_KEYMAP(XK_Next,       34, "PageDown");  ADD_KEYMAP(XK_End,        35, "End");
    ADD_KEYMAP(XK_Home,       36, "Home");      ADD_KEYMAP(XK_Left,       100, "ArrowLeft");
    ADD_KEYMAP(XK_Up,         101, "ArrowUp");   ADD_KEYMAP(XK_Right,      102, "ArrowRight");
    ADD_KEYMAP(XK_Down,       103, "ArrowDown"); ADD_KEYMAP(XK_Insert,     250, "Insert");
    ADD_KEYMAP(XK_Delete,     127, "Delete");    ADD_KEYMAP(XK_Super_L,    91,  "MetaLeft");
    ADD_KEYMAP(XK_Super_R,    92,  "MetaRight"); ADD_KEYMAP(XK_Menu,       93,  "ContextMenu");
    ADD_KEYMAP(XK_F1,         123, "F1");        ADD_KEYMAP(XK_F2,         124, "F2");
    ADD_KEYMAP(XK_F3,         125, "F3");        ADD_KEYMAP(XK_F4,         126, "F4");
    ADD_KEYMAP(XK_F5,         127, "F5");        ADD_KEYMAP(XK_F6,         128, "F6");
    ADD_KEYMAP(XK_F7,         129, "F7");        ADD_KEYMAP(XK_F8,         130, "F8");
    ADD_KEYMAP(XK_F9,         131, "F9");        ADD_KEYMAP(XK_F10,        132, "F10");
    ADD_KEYMAP(XK_F11,        133, "F11");       ADD_KEYMAP(XK_F12,        134, "F12");

    for (int i = 0; i < 26; ++i) { ADD_KEYMAP(XK_a + i, 'a' + i, std::string(1, 'a' + i)); }
    for (int i = 0; i < 10; ++i) { ADD_KEYMAP(XK_0 + i, '0' + i, std::string(1, '0' + i)); }
    
    ADD_KEYMAP(XK_minus, '-', "-"); ADD_KEYMAP(XK_equal, '=', "=");
    ADD_KEYMAP(XK_bracketleft, '[', "["); ADD_KEYMAP(XK_bracketright, ']', "]");
    ADD_KEYMAP(XK_semicolon, ';', ";"); ADD_KEYMAP(XK_apostrophe, '\'', "'");
    ADD_KEYMAP(XK_comma, ',', ",");     ADD_KEYMAP(XK_period, '.', ".");
    ADD_KEYMAP(XK_slash, '/', "/");     ADD_KEYMAP(XK_backslash, '\\', "\\");
}
    
EventDispatcher::EventHandler globalHandler;

void EventDispatcher::setGlobalHandler(EventHandler handler) {
    globalHandler = handler;
}

void EventDispatcher::pollAndTranslateEvents(Display* dpy) {
    while (XPending(dpy) > 0) {
        XEvent xevent;
        XNextEvent(dpy, &xevent);
        ProximaEvent p_event = translateEvent(xevent);
        if (p_event.type != PROXIMA_EVENT_NONE) {
            eventQueue.push(std::move(p_event));
        }
    }
}

void EventDispatcher::dispatchEvents() {
    while (!eventQueue.empty()) {
        ProximaEvent currentEvent = std::move(eventQueue.front());
        eventQueue.pop();
        
        if (globalHandler) {
            bool consumed = globalHandler(currentEvent);
            if (consumed) {
                // Event was handled by a component, do not process further for this event.
            }
        }
    }
}

ProximaEventKeyCode EventDispatcher::getKeySymCode(KeySym keysym) {
    if (keySymToCode.count(keysym)) {
        return keySymToCode[keysym];
    }
    return static_cast<ProximaEventKeyCode>(keysym); // Fallback to keysym value
}

std::string EventDispatcher::getKeySymString(KeySym keysym) {
    if (keySymToString.count(keysym)) {
        return keySymToString[keysym];
    }
    return ""; // Empty string if not mapped
}

ProximaEvent EventDispatcher::translateEvent(const XEvent& xevent) {
    ProximaEvent p_event; // Defaults to PROXIMA_EVENT_NONE

    switch (xevent.type) {
        case Expose:
            p_event.type = PROXIMA_EVENT_WINDOW_EXPOSE;
            // Note: Expose event often doesn't contain width/height directly useful for resize.
            // We'd typically get that from ConfigureNotify.
            break;

        case ConfigureNotify:
            p_event.type = PROXIMA_EVENT_WINDOW_RESIZE;
            p_event.data.window.width = xevent.xconfigure.width;
            p_event.data.window.height = xevent.xconfigure.height;
            break;

        case KeyPress:
            p_event.type = PROXIMA_EVENT_KEY_PRESS;
            p_event.data.key.keyCode = getKeySymCode(xevent.xkey.keycode);
            p_event.data.key.modifiers = xevent.xkey.state; // Raw X11 modifier state
            p_event.data.key.keyString = getKeySymString(xevent.xkey.keycode); // Use mapped string
            break;

        case KeyRelease:
            p_event.type = PROXIMA_EVENT_KEY_RELEASE;
            p_event.data.key.keyCode = getKeySymCode(xevent.xkey.keycode);
            p_event.data.key.modifiers = xevent.xkey.state;
            p_event.data.key.keyString = getKeySymString(xevent.xkey.keycode);
            break;

        case ButtonPress:
            p_event.type = PROXIMA_EVENT_MOUSE_BUTTON_PRESS;
            p_event.data.mouse.x = xevent.xbutton.x;
            p_event.data.mouse.y = xevent.xbutton.y;
            p_event.data.mouse.button = xevent.xbutton.button; // X11 button number
            p_event.data.mouse.modifiers = xevent.xbutton.state;
            break;

        case ButtonRelease:
            p_event.type = PROXIMA_EVENT_MOUSE_BUTTON_RELEASE;
            p_event.data.mouse.x = xevent.xbutton.x;
            p_event.data.mouse.y = xevent.xbutton.y;
            p_event.data.mouse.button = xevent.xbutton.button;
            p_event.data.mouse.modifiers = xevent.xbutton.state;
            break;

        case MotionNotify:
            p_event.type = PROXIMA_EVENT_MOUSE_MOTION;
            p_event.data.mouse.x = xevent.xmotion.x;
            p_event.data.mouse.y = xevent.xmotion.y;
            p_event.data.mouse.modifiers = xevent.xmotion.state;
            break;
        case ClientMessage:
            // This is where you'd check for the WM_DELETE_WINDOW atom if properly set up.
            // For now, let's trigger QUIT on Escape key press as a shortcut.
            // The ProximaEvent::QUIT is handled when its type is PROXIMA_EVENT_QUIT.
            // We'll simulate that below.
            break;
            
        default:
            // Ignore other event types for MVP
            break;
    }
    return p_event;
}