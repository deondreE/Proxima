#pragma once

#include "Window.hpp"	
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <functional>
#include "../Platform.hpp"

struct WinWindowDeleter {
  void operator()(HWND__* wnd) const {
	  if (wnd && IsWindow(wnd)) {
		std::cout << "DEBUG: Destroying window.... thingy\n";
        DestroyWindow(reinterpret_cast<HWND>(wnd));
	  }
  }
};

struct WinRendererDeleter {
  void operator()(HDC__* h) const {
	  if (h) {
		std::cout << "DEBUG:: Destroying HDC" << h << std::endl;
        DeleteDC(reinterpret_cast<HDC>(h));
	  }
  }
};

namespace UI {

class PEXPORT W_Window : public IWindow<HWND__, WinWindowDeleter, HDC__, WinRendererDeleter> {
public:	
	static constexpr const char* CLASS_NAME = "W_WindowClass";

	W_Window(const WindowConfig& config, View* rootView = nullptr);
    ~W_Window();

	void run() override;
	void stop() override; 

	bool initializePlatformSubsystems() override;
    void cleanupPlatformSubsystems() override;

protected:
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
											WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	WNDPROC _originalWndProc = nullptr;
	bool registerWindowClass();
	bool unregisterWindowClass();
};

} // Namespace UI