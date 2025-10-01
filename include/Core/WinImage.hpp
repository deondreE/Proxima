#pragma once
#include "IImage.hpp"
#include <Windows.h>
#include "Platform.hpp"

namespace Core {
class PEXPORT WinImage : public IImage {
  private:
    HBITMAP m_hBitMap = nullptr;
    int m_width = 0;
    int m_height = 0;
  
  public :
    WinImage(HBITMAP hBitmat, int width, int height) 
      : m_hBitMap(hBitmat), m_width(width), m_height(height){}
    ~WinImage() override {
      if (m_hBitMap) {
        DeleteObject(m_hBitMap);
        m_hBitMap = nullptr;
      }
    };

    int getWidth() const override { return m_width; }
    int getHeight() const override { return m_height; }

    HBITMAP getMap() const { return m_hBitMap; }
};
} // Core