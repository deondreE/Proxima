#include "UI/WIN_Renderer.hpp"

namespace UI {

	WIN_Renderer::WIN_Renderer(HWND hWnd, int initialWidth, int initialHeight)
    : m_hWnd(hWnd), // Correctly initialize m_hWnd
      m_hMemoryDC(nullptr), m_hBitmap(nullptr), m_hOldBitmap(nullptr),
      m_hPen(nullptr), m_hBrush(nullptr),
      m_currentColor(255, 255, 255, 255), // Default to black
      m_width(initialWidth), m_height(initialHeight),
			m_hBackgroundBrush(nullptr)
{
    if (!m_hWnd || !IsWindow(m_hWnd)) {
        throw std::runtime_error("WIN_Renderer: Invalid window handle provided to constructor.");
    }
    
    createBackBuffer(m_width, m_height);

    updateGDIObjects();

		m_hBackgroundBrush = CreateSolidBrush(RGB(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b));
    if (!m_hBackgroundBrush) {
        // Handle error: could throw or log
        OutputDebugStringA("Failed to create initial background brush!\n");
    }

    _textRenderer = std::make_unique<Core::WinTextRenderer>(m_hMemoryDC);
}

WIN_Renderer::~WIN_Renderer() {
    if (m_hPen) DeleteObject(m_hPen);
    if (m_hBrush) DeleteObject(m_hBrush);
		if (m_hBackgroundBrush) DeleteObject(m_hBackgroundBrush);
    
    destroyBackBuffer();
}

void WIN_Renderer::createBackBuffer(int width, int height) {
    HDC hScreenDC = GetDC(m_hWnd); 
    if (!hScreenDC) {
        throw std::runtime_error("WIN_Renderer: Failed to get screen DC for back buffer creation.");
    }

    m_hMemoryDC = CreateCompatibleDC(hScreenDC);
    if (!m_hMemoryDC) {
        ReleaseDC(m_hWnd, hScreenDC);
        throw std::runtime_error("WIN_Renderer: Failed to create compatible memory DC.");
    }

    m_hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    if (!m_hBitmap) {
        DeleteDC(m_hMemoryDC);
        ReleaseDC(m_hWnd, hScreenDC);
        throw std::runtime_error("WIN_Renderer: Failed to create compatible bitmap.");
    }

    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemoryDC, m_hBitmap); // Select bitmap into memory DC

    ReleaseDC(m_hWnd, hScreenDC); // Release screen DC immediately

    m_width = width;
    m_height = height;
}

	void WIN_Renderer::setDrawColor(const Color& color) {
      if (m_currentColor.r != color.r || m_currentColor.g != color.g ||
          m_currentColor.b != color.b || m_currentColor.a != color.a) {
        m_currentColor = color;
        updateGDIObjects();  
      }
	}

void WIN_Renderer::updateGDIObjects() {
    if (!m_hMemoryDC) return; // No DC to select into

    // Restore old objects if they were selected, then delete them
    if (m_hPen) {
        // HGDIOBJ oldPen = SelectObject(m_hMemoryDC, GetStockObject(BLACK_PEN)); // Restore to default if needed
        DeleteObject(m_hPen);
        m_hPen = nullptr;
    }
    if (m_hBrush) {
        // HGDIOBJ oldBrush = SelectObject(m_hMemoryDC, GetStockObject(WHITE_BRUSH)); // Restore to default
        DeleteObject(m_hBrush);
        m_hBrush = nullptr;
    }
    
    COLORREF cref = RGB(m_currentColor.r, m_currentColor.g, m_currentColor.b);
    m_hPen = CreatePen(PS_SOLID, 1, cref); // 1 pixel wide solid pen
    m_hBrush = CreateSolidBrush(cref);     // Solid brush

    // Selects into the MEMORY DC (m_hMemoryDC)
    if (m_hPen) SelectObject(m_hMemoryDC, m_hPen);
    if (m_hBrush) SelectObject(m_hMemoryDC, m_hBrush);
}

void WIN_Renderer::destroyBackBuffer() {
	if (m_hMemoryDC) {
			SelectObject(m_hMemoryDC, m_hOldBitmap); // Restore original bitmap to DC
			DeleteDC(m_hMemoryDC);
			m_hMemoryDC = nullptr;
	}
	if (m_hBitmap) {
			DeleteObject(m_hBitmap);
			m_hBitmap = nullptr;
	}
}

void WIN_Renderer::resize(int width, int height) {
    if (width == m_width && height == m_height) return;
    if (width <= 0 || height <= 0) return; // Prevent creating invalid bitmaps

    destroyBackBuffer();
    createBackBuffer(width, height);
    updateGDIObjects();
}

void WIN_Renderer::drawRect(int x, int y, int w, int h) {
    if (!m_hMemoryDC) return;
    // GDI Rectangle draws a border with current pen, fills with current brush
    Rectangle(m_hMemoryDC, x, y, x + w, y + h);
}

void WIN_Renderer::fillRect(int x, int y, int w, int h) {
    if (!m_hMemoryDC) return;
    RECT rect{ x, y, x + w, y + h };
    FillRect(m_hMemoryDC, &rect, m_hBrush); // Fill with current brush
}

void WIN_Renderer::drawLine(int x1, int y1, int x2, int y2) {
    if (!m_hMemoryDC) return;
    MoveToEx(m_hMemoryDC, x1, y1, nullptr); // Set starting point
    LineTo(m_hMemoryDC, x2, y2);            // Draw line to end point
}

void WIN_Renderer::clear() {
    if (!m_hMemoryDC) return;
    RECT rect{0, 0, m_width, m_height};
    FillRect(m_hMemoryDC, &rect, m_hBackgroundBrush); 
}

void WIN_Renderer::present() {
    if (!m_hWnd || !m_hMemoryDC) return;

    HDC hScreenDC = GetDC(m_hWnd); // Get the screen DC to blit to
    if (hScreenDC) {
        BitBlt(hScreenDC, 0, 0, m_width, m_height, m_hMemoryDC, 0, 0, SRCCOPY);
        ReleaseDC(m_hWnd, hScreenDC); // Release screen DC
    }
    GdiFlush();
}

} // Namespace UI