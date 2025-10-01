#pragma once
#include "Core/IEventDispatcher.hpp"
#include "Windows.h"
#include <queue>
#include <array>

namespace Core {

	class WinEventDispatcher : public IEventDispatcher {
		std::queue<ProximaEvent> _q; 
		std::array<Handler, TEXT_INPUT + 1> _handlers{};

	public:
		void onQuit(Handler h) override { _handlers[QUIT] = std::move(h); }
        void onKeyPress(Handler h) override {
          _handlers[KEY_PRESS] = std::move(h);
        }
        void onKeyRelease(Handler h) override {
          _handlers[KEY_RELEASE] = std::move(h);
        }
        void onMousePress(Handler h) override {
          _handlers[MOUSE_PRESS] = std::move(h);
        }
        void onMouseRelease(Handler h) override {
          _handlers[MOUSE_RELEASE] = std::move(h);
        }
        void onMouseMotion(Handler h) override {
          _handlers[MOUSE_MOTION] = std::move(h);
        }
        void onWindowResize(Handler h) override {
          _handlers[WINDOW_RESIZE] = std::move(h);
        }
        void onTextInput(Handler h) override {
          _handlers[TEXT_INPUT] = std::move(h);
        }

		void pollAndTranslate() override {
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
          _q.push(ProximaEvent{QUIT});
        } else {
          ProximaEvent e;
          e.type = UNKNOWN_EVENT;

          switch (msg.message) {
            case WM_KEYDOWN: 
              e.type = KEY_PRESS; 
              e.keyCode = (int)msg.wParam;
              break;
            case WM_KEYUP:
              e.type = KEY_RELEASE;
              e.keyCode = (int)msg.wParam;
              break;
            case WM_LBUTTONDOWN:
              e.type = MOUSE_PRESS;
              e.x = GET_X_LPARAM(msg.lParam); 
              e.y = GET_Y_LPARAM(msg.lParam);
              e.button = 1;
              break;
            case WM_LBUTTONUP:
              e.type = MOUSE_RELEASE;
              e.x = GET_X_LPARAM(msg.lParam); 
              e.y = GET_Y_LPARAM(msg.lParam);
              e.button = 1;
              break;
            case WM_MOUSEMOVE:
              e.type = MOUSE_MOTION;
              e.x = GET_X_LPARAM(msg.lParam); 
              e.y = GET_Y_LPARAM(msg.lParam);
              break;
            case WM_SIZE:
              e.type = WINDOW_RESIZE;
              e.width = LOWORD(msg.lParam);
              e.height = HIWORD(msg.lParam);
              break;
          }

          if (e.type != UNKNOWN_EVENT) {
            _q.push(e);
          }

          TranslateMessage(&msg);
          DispatchMessage(&msg); 
        }
		  }
		}

		void dispatch() override { 
			while (!_q.empty()) {
            auto e = _q.front();
            _q.pop();

            if (e.type >= 0 && e.type < _handlers.size()) {
              if (_handlers[e.type])
                _handlers[e.type](e);
            }
           
			}
		}
	};

} // Core