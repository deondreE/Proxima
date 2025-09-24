#pragma once
#include <string>
#include <X11/Xlib.h>

typedef unsigned int ProximaEventKeyCode; 
typedef unsigned int ProximaEventModifiers; 
typedef unsigned int ProximaEventMouseButton;

enum ProximaEventType {
    PROXIMA_EVENT_NONE,
    PROXIMA_EVENT_QUIT,
    PROXIMA_EVENT_KEY_PRESS,
    PROXIMA_EVENT_KEY_RELEASE,
    PROXIMA_EVENT_MOUSE_BUTTON_PRESS,
    PROXIMA_EVENT_MOUSE_BUTTON_RELEASE,
    PROXIMA_EVENT_MOUSE_MOTION,
    PROXIMA_EVENT_WINDOW_RESIZE,
    PROXIMA_EVENT_WINDOW_EXPOSE 
};

struct ProximaEvent {
    ProximaEventType type = PROXIMA_EVENT_NONE;

    union Data
    {
        struct {
            ProximaEventKeyCode keyCode;
            ProximaEventModifiers modifiers;
            std::string keyString;
        } key;
        
        struct {
            int x, y;
            ProximaEventMouseButton button;
            ProximaEventModifiers modifiers;
        } mouse;

        struct {
            int width, height;
        } window;

        Data() {}
        ~Data() {}

        Data(const Data&) = delete;
        Data& operator=(const Data&) = delete;
        Data& operator=(Data&& other) noexcept { // Move assignment
            if (this != &other) {
                // Destruct existing members if necessary (not needed for this union layout)
                
                this->key.keyCode = other.key.keyCode;
                this->key.modifiers = other.key.modifiers;
                this->key.keyString = std::move(other.key.keyString);

                this->mouse.x = other.mouse.x;
                this->mouse.y = other.mouse.y;
                this->mouse.button = other.mouse.button;
                this->mouse.modifiers = other.mouse.modifiers;

                this->window.width = other.window.width;
                this->window.height = other.window.height;
            }
            return *this;
        }

         Data(Data&& other) noexcept { // Move constructor
            this->key.keyCode = other.key.keyCode;
            this->key.modifiers = other.key.modifiers;
            this->key.keyString = std::move(other.key.keyString);

            this->mouse.x = other.mouse.x;
            this->mouse.y = other.mouse.y;
            this->mouse.button = other.mouse.button;
            this->mouse.modifiers = other.mouse.modifiers;

            this->window.width = other.window.width;
            this->window.height = other.window.height;
         }
    } data;

    ProximaEvent() : type(PROXIMA_EVENT_NONE) {}
    ProximaEvent(const ProximaEvent&) = delete;
    ProximaEvent& operator=(const ProximaEvent&) = delete;
    ProximaEvent(ProximaEvent&& other) noexcept : type(other.type), data(std::move(other.data)) {}
    ProximaEvent& operator=(ProximaEvent&& other) noexcept {
        if (this != &other) {
            type = other.type;
            data = std::move(other.data); // Move the union data
        }
        return *this;
    }
};