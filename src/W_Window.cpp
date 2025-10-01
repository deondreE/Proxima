#include "UI/W_Window.hpp"
#include "Core/WinEventDispatcher.hpp" 
#include "UI/WIN_Renderer.hpp"

namespace UI {
	static W_Window* s_currentCreatingWindow = nullptr;

	W_Window::W_Window(const WindowConfig& config, View* rootView)
		: IWindow(nullptr, WinWindowDeleter(),
			rootView, config, 0) 
	{
      _titleBarHeight =
          GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME);
      
      if (!rootView) {
        _rootView = std::make_unique<View>();
      }
      _rootView->size(_config.initialWidth, _config.initialHeight);
	}

	W_Window::~W_Window() {
      cleanupPlatformSubsystems();
	}

	bool W_Window::registerWindowClass() {
      WNDCLASSEX wc = {};
      wc.cbSize = sizeof(WNDCLASSEX);
      wc.lpfnWndProc = W_Window::StaticWndProc;
      wc.hInstance = GetModuleHandle(nullptr);
      wc.lpszClassName = CLASS_NAME;
      wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
      wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);

      if (!RegisterClassEx(&wc)) {
          std::cerr << "ERROR: Failed to register window class! Error "
                    << GetLastError() << std::endl;
          return false;
      }
      return true;
	}

    bool W_Window::unregisterWindowClass() {
      return UnregisterClass(CLASS_NAME, GetModuleHandle(nullptr)) != 0;
    }

    bool W_Window::initializePlatformSubsystems() {
        if (!registerWindowClass()) {
            return false;
          }

        s_currentCreatingWindow = this;

        HWND hWnd = CreateWindowEx(
            _config.exStyle,
            CLASS_NAME,
            _config.title.c_str(),
            _config.style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            _config.initialWidth, _config.initialHeight,
            nullptr,
            nullptr, GetModuleHandle(nullptr),
            this
        );
        s_currentCreatingWindow = nullptr;

        if (!hWnd) {
            unregisterWindowClass();
            std::cerr << "ERROR: Failed to create window! Error " << GetLastError() << std::endl;
            return false;
        }
        _window.reset(hWnd);

        _eventDispatcher = std::make_unique<Core::WinEventDispatcher>();
        _renderer = std::make_unique<WIN_Renderer>(_window.get(), _config.initialWidth, _config.initialHeight);

        if (_rootView) {
          ViewContext initialContext;
          initialContext.renderer = _renderer.get();
          _rootView->setContext(initialContext);
        }

        setupEventHandlers();

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        return true;
    }

    void W_Window::setupEventHandlers() {
      auto genericHandler = [this](const ProximaEvent& e) {
        if (_rootView) {
            _rootView->handleProximaEvent(e);
            return true;
        }

        return false;
      };

      _eventDispatcher->onQuit([this](const ProximaEvent& e){ _running = false; return false; }); // Specific handler for Quit
      _eventDispatcher->onKeyPress(genericHandler);
      _eventDispatcher->onKeyRelease(genericHandler);
      _eventDispatcher->onMousePress(genericHandler);
      _eventDispatcher->onMouseRelease(genericHandler);
      _eventDispatcher->onMouseMotion(genericHandler);
      _eventDispatcher->onWindowResize(genericHandler);
      _eventDispatcher->onTextInput(genericHandler);
    }

    void W_Window::run() {
        if (!_window) {
            std::cerr << "ERROR: Cannot run W_Window, window not initialized!"
                  << std::endl;
          return;
        }

        MSG msg = {};
        _running = true;

        float deltaTime = 0.016f;

        while (_running) {

          ViewContext frameContext;
          frameContext.renderer = _renderer.get();

          _eventDispatcher->pollAndTranslate();
          if (!_running) break; 
          _eventDispatcher->dispatch(); 

          // Update
          if (_rootView) {
            _rootView->update(deltaTime, frameContext);
          }

          // Layout
          if (_rootView) {
            _rootView->layout(0, 0);
          }

          // Clear & draw
          if (_rootView) {
            try {
              _rootView->draw(frameContext);
            } catch(std::exception& ex) {
              std::cerr << "Root view draw exception: " << ex.what() << std::endl;
            }
          }

          _renderer->present();
        }
        
        std::cout << "DEBUG:= Exited WinAPI message loop." << std::endl;
    }

    void W_Window::stop() {
      std::cout << "DEBUG: Stopping requested" << std::endl;
      _running = false;
    }

    void W_Window::cleanupPlatformSubsystems() {
      unregisterWindowClass();
      std::cout << "DEBUG: W_Window platform clean." << std::endl;
    }

    LRESULT CALLBACK W_Window::StaticWndProc(HWND hWnd, UINT message,
                                        WPARAM wParam, LPARAM lParam) {
    W_Window* pThis;
    if (message == WM_NCCREATE) {
        auto pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = reinterpret_cast<W_Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    pThis = reinterpret_cast<W_Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (pThis) {
        return pThis->WndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

    LRESULT W_Window::WndProc(HWND hWnd, UINT message, WPARAM wParam,
        LPARAM lParam) {
        switch (message) { 
        case WM_CLOSE:
            PostQuitMessage(0);
             return 0;
            break;
        case WM_PAINT: {
          ValidateRect(hWnd, nullptr);
          break;
        }
        case WM_LBUTTONDOWN:
          std::cout << "DEBUG: Left mouse button down at ("
                    << GET_X_LPARAM(lParam) << ", " << GET_Y_LPARAM(lParam)
                    << ")" << std::endl;
          return 0;
          break;
        case WM_DESTROY:
          PostQuitMessage(0);
          return 0;
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
        }
        
         return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // Namespace UI