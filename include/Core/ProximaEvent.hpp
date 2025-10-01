#pragma once
#include <string>
#include "Platform.hpp"

typedef PEXPORT unsigned int ProximaEventKeyCode;
typedef PEXPORT unsigned int ProximaEventModifiers;
typedef PEXPORT unsigned int ProximaEventMouseButton;

enum PEXPORT ProximaEventType {
  NONE,
  QUIT,
  KEY_PRESS,
  KEY_RELEASE,
  MOUSE_PRESS,
  MOUSE_RELEASE,
  MOUSE_MOTION,
  WINDOW_RESIZE,
  TEXT_INPUT,
  UNKNOWN_EVENT,
};

enum PEXPORT ProximaKeyCode {
    KEY_A, KEY_B, KEY_C,
    KEY_D, KEY_E, KEY_F,
    KEY_G, KEY_H, KEY_I,
    KEY_J, KEY_K, KEY_L,
    KEY_M, KEY_N, KEY_O,
    KEY_P, KEY_Q, KEY_R,
    KEY_S, KEY_T, KEY_U,
    KEY_V, KEY_W, KEY_X,
    KEY_Y, KEY_Z,
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
    KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT,
    KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL,
    KEY_LEFT_ALT, KEY_RIGHT_ALT,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_TAB,
    KEY_SPACE,
    KEY_BACKSPACE,
    KEY_DELETE, 
    KEY_ARROW_LEFT, KEY_ARROW_RIGHT, KEY_ARROW_UP, KEY_ARROW_DOWN,
};

struct PEXPORT ProximaEvent {
  ProximaEventType type = NONE;

  ProximaKeyCode  keyCode;
  ProximaEventModifiers modifiers = 0;
  std::string          text;        // e.g. UTF‐8 for key
  std::wstring w_text = L"";

  int                  x = 0, y = 0;
  ProximaEventMouseButton button = 0;

  int                  width = 0, height = 0;
};
