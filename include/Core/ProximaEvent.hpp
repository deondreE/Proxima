#pragma once
#include <string>

typedef unsigned int ProximaEventKeyCode;
typedef unsigned int ProximaEventModifiers;
typedef unsigned int ProximaEventMouseButton;

enum ProximaEventType {
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

struct ProximaEvent {
  ProximaEventType type = NONE;

  // common fields
  ProximaEventKeyCode  keyCode    = 0;
  ProximaEventModifiers modifiers = 0;
  std::string          text;        // e.g. UTF‐8 for key
  std::wstring w_text = L"";

  int                  x = 0, y = 0;
  ProximaEventMouseButton button = 0;

  int                  width = 0, height = 0;
};
