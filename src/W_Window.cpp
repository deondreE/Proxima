#include "UI/W_Window.hpp"
#include "Core/WinEventDispatcher.hpp" 

namespace UI {
	static W_Window* s_currentCreatingWindow = nullptr;

	W_Window::W_Window(const WindowConfig& config, View* rootView)
		: IWindow(nullptr, WinWindowDeleter(), nullptr, WinRendererDeleter(),
			rootView, config, 0) 
	{
      _titleBarHeight =
          GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME);
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
          return false;
        }
        _window.reset(hWnd);

        HDC hdc = GetDC(hWnd);
        HDC compatibleHdc = CreateCompatibleDC(hdc);
        if (compatibleHdc) {
          // _renderer.reset();
        } else {
          std::cerr << "WARNING: Failed to create compatible HDC; Other "
                      "words.... Broken Fix it!";
        }

        _eventDispatcher = std::make_unique<Core::WinEventDispatcher>();

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        return true;
    }

    void W_Window::run() {
        if (!_window) {
            std::cerr << "ERROR: Cannot run W_Window, window not initialized!"
                  << std::endl;
          return;
        }

        MSG msg = {};
        _running = true;


        while (_running) {
          pumpEvents();
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
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hWnd, &ps);

          RECT clientRect;
          GetClientRect(hWnd, &clientRect);
          HBRUSH brush = CreateSolidBrush(RGB(128, 0, 128));
          FillRect(hdc, &clientRect, brush);
          DeleteObject(brush);

          std::string text = "Hello from W_Window!";
          SetTextColor(hdc, RGB(255, 255, 0));
          SetBkMode(hdc, TRANSPARENT);
          DrawText(hdc, text.c_str(), -1, &clientRect,
                  DT_SINGLELINE | DT_CENTER | DT_VCENTER);

          EndPaint(hWnd, &ps);
          return 0;
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