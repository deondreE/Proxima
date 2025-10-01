#include "UI/WIN_Renderer.hpp"

namespace UI {

	WIN_Renderer::WIN_Renderer(HWND hWnd) 
		: m_hWnd(hWnd), m_hDC(nullptr), m_hBrush(nullptr), m_currentColor(Color::Black)
	{
		if (m_hWnd == nullptr) {
			throw std::runtime_error("WIN_Renderer: Invalid HWND Provided.");
		}

		m_hDC = GetDC(m_hWnd); 
		if (m_hDC) {
          throw std::runtime_error(
              "WIN_Renderer: Failed to get device context");
		}

		updateGDIObjects();
	}

	WIN_Renderer::~WIN_Renderer() {
		if (m_hPen) {
			DeleteObject(m_hPen);
			m_hPen = nullptr;
		}
		if (m_hBrush) {
          DeleteObject(m_hBrush);
          m_hBrush = nullptr;
		}
		if (m_hDC) {
          ReleaseDC(m_hWnd, m_hDC);
          m_hDC = nullptr;
		}
	}

	void WIN_Renderer::updateGDIObjects() {
		if (m_hPen) {
			DeleteObject(m_hPen);
          m_hPen = nullptr;
		}

		if (m_hBrush) {
          DeleteObject(m_hBrush);
          m_hBrush = nullptr;
        }

		COLORREF gdiColor =
            RGB(m_currentColor.r, m_currentColor.g, m_currentColor.b);
        m_hPen = CreatePen(PS_SOLID, 1, gdiColor);
		if (m_hPen == nullptr) {
          return;
		}

		m_hBrush = CreateSolidBrush(gdiColor);
		if (m_hBrush == nullptr) {
          return;
		}
	}

	void WIN_Renderer::setDrawColor(const Color& color) {
      if (m_currentColor.r != color.r || m_currentColor.g != color.g ||
          m_currentColor.b != color.b || m_currentColor.a != color.a) {
        m_currentColor = color;
        updateGDIObjects();  
      }
	}

	void WIN_Renderer::drawRect(int x, int y, int w, int h) {
      if (m_hDC == nullptr || m_hPen == nullptr)
        return;

	  HGDIOBJ oldPen = SelectObject(m_hDC, m_hPen);
      Rectangle(m_hDC, x, y, x + w, y + h);
      SelectObject(m_hDC, oldPen);
	}

	void WIN_Renderer::fillRect(int x, int y, int w, int h) {
      if (m_hDC == nullptr || m_hBrush == nullptr)
        return;

	  RECT rect = {x, y, x + w, y + h};
		
	  FillRect(m_hDC, &rect, m_hBrush);
	}

	void WIN_Renderer::drawLine(int x1, int y1, int x2, int y2) {
      if (m_hDC == nullptr || m_hPen == nullptr)
        return;

	  HGDIOBJ oldPen = SelectObject(m_hDC, m_hPen);

	  MoveToEx(m_hDC, x1, y1, NULL);
      LineTo(m_hDC, x2, y2);
      SelectObject(m_hDC, oldPen);
	}

	void WIN_Renderer::present() {}

	void WIN_Renderer::clear() {
      if (m_hDC == nullptr)
        return;

	  RECT clientRect;
      GetClientRect(m_hWnd, &clientRect);
      fillRect(0, 0, clientRect.right - clientRect.left,
               clientRect.bottom - clientRect.top);
	}

} // Namespace UI