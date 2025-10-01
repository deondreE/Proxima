#pragma once

#include <Windows.h>
#include "UI/Renderer.hpp"
#include <stdexcept>

namespace UI {

class WIN_Renderer : public Renderer {
public:
    explicit WIN_Renderer(HWND hWnd);
	virtual ~WIN_Renderer();

	void setDrawColor(const Color& color) override;
	void drawRect(int x, int y, int w, int h) override;
	void fillRect(int x, int y, int w, int h) override;
	void drawLine(int x1, int y1, int x2, int y2) override;

	void present() override;
	void clear() override;

private:
 HWND m_hWnd;
 HDC m_hDC;
 HPEN m_hPen;
 HBRUSH m_hBrush;
 Color m_currentColor;

 void updateGDIObjects();
};

} // Namespace UI