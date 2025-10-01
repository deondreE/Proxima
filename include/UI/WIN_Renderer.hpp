#pragma once

#include <Windows.h>
#include "UI/Renderer.hpp"
#include "Core/WinTextRenderer.hpp"
#include "Core/WinImage.hpp"
#include "Core/WinImageLoader.hpp"
#include <stdexcept>
#include <memory>
#include "Platform.hpp"

namespace UI {

class PEXPORT WIN_Renderer : public Renderer {
public:
  explicit WIN_Renderer(HWND hWnd, int initialWidth, int initialHeight);
	virtual ~WIN_Renderer();

	void setDrawColor(const Color& color) override;
	void drawRect(int x, int y, int w, int h) override;
	void fillRect(int x, int y, int w, int h) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
 void drawImage(Core::IImage* image, int srcX, int srcY, int srcH, int srcW, int destX, int destY, int destW, int destH) override;

	void present() override;
	void clear() override;

	void resize(int width, int height);

	Core::TextRenderer* getTextRenderer() const override {
		return _textRenderer.get();
	}

	Core::IImageLoader* getImageLoader() const override {
		return _imageLoader.get();
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
	std::unique_ptr<Core::WinImageLoader> _imageLoader;

  void updateGDIObjects();
 	void createBackBuffer(int width, int height);
 	void destroyBackBuffer();
};

} // Namespace UI