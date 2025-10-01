#pragma once

#include <Windows.h>
#include "UI/Renderer.hpp"
#include "Core/WinTextRenderer.hpp"
#include <stdexcept>
#include <memory>

namespace UI {

class WIN_Renderer : public Renderer {
public:
  explicit WIN_Renderer(HWND hWnd, int initialWidth, int initialHeight);
	virtual ~WIN_Renderer();

	void setDrawColor(const Color& color) override;
	void drawRect(int x, int y, int w, int h) override;
	void fillRect(int x, int y, int w, int h) override;
	void drawLine(int x1, int y1, int x2, int y2) override;

	void present() override;
	void clear() override;

	void resize(int width, int height);

	Core::TextRenderer* getTextRenderer() const override {
		return _textRenderer.get();
	}

private:
	HWND m_hWnd;
  HDC m_hMemoryDC;
	RECT m_clientBounds;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_width, m_height;

	HPEN m_hPen;
	HBRUSH m_hBackgroundBrush;
	Color m_backgroundColor;
	HBRUSH m_hBrush;
	Color m_currentColor;
	std::unique_ptr<Core::WinTextRenderer> _textRenderer;

  void updateGDIObjects();
 	void createBackBuffer(int width, int height);
 	void destroyBackBuffer();
};

} // Namespace UI